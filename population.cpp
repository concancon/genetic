#include "population.h"
#include <chrono>
#include <iostream>
#include <math.h>
#include <numeric>
#include <random>

using namespace std;
using namespace std::chrono;
using namespace c74::max;

struct LightIterator : public std::vector<DNA>::iterator {
    LightIterator(std::vector<DNA>::iterator it)
        : std::vector<DNA>::iterator(it) {}
    double &operator*() {
        return std::vector<DNA>::iterator::operator*().fitness;
    }
};

// create a population with default values for mutation rate and population
// size. calculate the value of the perfect PARAMS: target params: input from
// max. For example a set of pixels
Population::Population(const vector<double> &tp)
    : popDict(c74::min::symbol(true)), counter(256) {

    maxDict = (t_object *)popDict; // produces a memory leak //TODO: CREATE
                                   // DESTRUCTOR TO RELEASE THIS
    expFactor = 0.204;
    targetParams = tp;
    finished = false;
    population.clear();
    mutationRate = 0.001;
    maxPopulation = 10;
    mutationIndex = 10;
    numParams = targetParams.size();

    for (int i = 0; i < maxPopulation; i++) {
        DNA dna(numParams, true);
        population.push_back(std::move(dna));
    }

#if USE_THREADS
    for (int i = 0; i < numThreads; i++) {
        workers[i] = std::make_unique<WorkerThread>();
    }
#endif
    cout << "our old constructor has been called" << endl;
}

Population::Population(int numberOfParams)
    : popDict(c74::min::symbol(true)), counter(256) {

    maxDict = (t_object *)popDict; // produces a memory leak //TODO: CREATE
                                   // DESTRUCTOR TO RELEASE THIS
    expFactor = 0.123;
    finished = false;
    population.clear();
    mutationRate = 0.214;
    mutationIndex = 5.;
    maxPopulation = 10;
    accuracy = DEFAULT_ACCURACY; //TOODO: implement macros for all these values
    numParams = numberOfParams;

    for (int i = 0; i < maxPopulation; i++) {
        DNA dna(numberOfParams, true);
        population.push_back(std::move(dna));
    }
}


void Population::adoptAttributes(const Population &pop){

    
     mutationRate = pop.mutationRate;
     mutationIndex= pop.mutationIndex;
     expFactor = pop.expFactor;
     maxPopulation= pop.maxPopulation;
     accuracy= pop.accuracy;
     

}



// converts a population to an atomarray
const c74::min::dict &Population::toDict() {

    t_dictionary *d = dictionary_new();
    long idx = 0;
    string basename = "pop_";

    for (auto pop : population) {
        string keyname = basename + to_string(idx++);
        dictionary_appendatomarray(d, gensym(keyname.c_str()),
                                   (t_object *)pop.toAtomArray());
    }
    dictionary_appendlong(maxDict, gensym("generation"), generations);

    dictionary_appenddictionary(maxDict, gensym("population"), (t_object *)d);

    return popDict;
}

// setter for the population size
void Population::setMaxPopulation(int mp) {
    maxPopulation = mp;
    generations = 0;
    population.clear();
    for (int i = 0; i < maxPopulation; i++) {
        DNA dna(numParams, true);
        population.push_back(std::move(dna));
    }

}

void Population::setExpFactor(double ef) { expFactor = ef; }

void Population::setAccuracy(double a) { accuracy = a; }
// uncomment to time the function, it's definitely faster
// #define BENCHMARK

// Compute average fitness for the population
double Population::getAverageFitness() {
    double total = 0.0;
    total = std::accumulate(LightIterator{population.begin()},
                            LightIterator{population.end()}, (double)0.0);

    return (total / (double)population.size());
}
// get the fittest member in the population.
// we use this to output the best member to Max
vector<int> &Population::getBest(int &index) {
    

    static vector<int> defaultGenes = {-1};
    vector<int> diff;

    maxFitness = 0.;
    index = -1;
    for (int i = 0; i < population.size(); i++) {
        if (population[i].fitness > maxFitness) {
            maxFitness = population[i].fitness;
            index = i;
        }
    }

    if (maxFitness >= accuracy) {
        
        finished = true;
    }
    if (index >= 0) {

        diff.clear();

        // calculate the amount of elements that have changed since last call
        std::set_difference(population[index].genes.begin(),
                            population[index].genes.end(), lastBest.begin(),
                            lastBest.end(), std::inserter(diff, diff.begin()));

        rateOfImprovement =
            (double)diff.size() / (double)population[index].genes.size();

        lastBest = population[index].genes;
        return population[index].genes;
    }
    return defaultGenes;
}

void Population::calcFitness() {
    if (targetParams.size()) {
        for (auto it = population.begin(); it < population.end(); it++) {
            it->fitnessFunction(targetParams);
        }
    }
}
// This method replaces what used to be the natural selection method.
// it is based on the idea of setting each fitness value to a percentage in
// relation to the sum of all individuals.
// This percentage is then used when selecting a score, whereby a member with a
// higher score has a greater probability of being chosen.
void Population::generate(double mutationIndex) {

    double sum = 0.;
    // Refill the population with children from the mating pool
    newPopulation.clear();

    std::sort(population.begin(), population.end(),
              [](const DNA &a, const DNA &b) -> bool {
                  return a.fitness > b.fitness;
              });
    int elitelen = population.size() * 0.1;
    for (int i = 0; i < elitelen; i++) {
        newPopulation.push_back(population[i]);
    }

    exponentialRanker(expFactor);
    for (int i = 0; i < population.size(); i++) {
        sum += probabilityArray[i];
    }

    for (int i = 0; i < population.size() - elitelen; i++) {
        DNA partnerA = rSelect();
        DNA partnerB = rSelect();
        DNA child = partnerA.crossover(
            partnerB); 
        child.mutate(mutationRate, mutationIndex);
        newPopulation.push_back(std::move(child));
    }
    population.swap(newPopulation);
    // population= newPopulation;
    generations++;

    // calcFitness();
}

// Creates probability array which maps raw fitness values to values which have
// a hyper exponential curve.
// @Param c defines steepness of curve. where c must within the range [0..1).
// A small value of c increases the probability of the best phenotypes to be
// selected. If c is set to zero, the selection probability of the best phenotype
// is set to one. The selection probability of all other phenotypes is zero. A
// value near one equalizes the selection probabilities.

//TOODO: RENAME TO exponentialRanker
void Population::exponentialRanker(double c) {

    probabilityArray.clear();
    // first we need to sort the array in descending order
    // std::sort(population.begin(), population.end(), [](const DNA& a, const
    // DNA& b) -> bool { return a.fitness > b.fitness; });

    for (int i = 0; i < population.size(); i++) {

        double numerator = c - 1;
        double denominator = pow(c, population.size()) - 1.;
        double probability = pow(c, i) * (numerator / denominator);
        probabilityArray.push_back(probability);
    }

}
// select based on exponentialSelector
DNA &Population::select(double sum) {

    double random = (double)equalRandom(gen) * sum;

    int index = 0;
    for (; random > 0. && index < probabilityArray.size(); index++) {
        random -= probabilityArray[index];
    }
    if (random > 0.0) {
        cout << "random is greater than 0!" << endl;
    }
    index--;

    population[index].count++;
    return population[index];
}

DNA &Population::rSelect() {
    double rndNumber = rand() / (double)RAND_MAX;
    double offset = 0.0;
    int pick = 0;

    for (int i = 0; i < population.size(); i++) {
        offset += probabilityArray[i];
        if (rndNumber < offset) {
            pick = i;
            break;
        }
    }
    
    return population[pick];
}

vector<double> &Population::displayPopulation() {
    for (int i = 0; i < population.size(); i++) {
        for (int j = 0; j < population[i].genes.size(); j++) {
            // count the occurence of each gene
            counter[population[i].genes[j]]++;
        }
    }
    return counter;
}

int Population::getNumberOfParams() { return numParams; }

vector<int> Population::getSelectionCount() {
    vector<int> numberOfTimesSelected;
    for (int i = 0; i < population.size(); i++) {
        numberOfTimesSelected.push_back(population[i].count);
    }

    return numberOfTimesSelected;
}
