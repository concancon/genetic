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
    : popDict(c74::min::symbol(true)) {

    maxDict = (t_object *)popDict; // produces a memory leak //TODO: CREATE
                                   // DESTRUCTOR TO RELEASE THIS
    targetParams = tp;
    finished = false;
    populationMembers.clear();
    numParams = targetParams.size();

	for (int i = 0; i < getMaxPopulation(); i++) {
        DNA dna(numParams, true);
        populationMembers.push_back(std::move(dna));
    }

    cout << "our old constructor has been called" << endl;
}

Population::Population(int numberOfParams)
    : popDict(c74::min::symbol(true)){

    maxDict = (t_object *)popDict; // produces a memory leak //TODO: CREATE
                                   // DESTRUCTOR TO RELEASE THIS
    finished = false;
    populationMembers.clear();
    numParams = numberOfParams;

	for (int i = 0; i < getMaxPopulation(); i++) {
        DNA dna(numberOfParams, true);
        populationMembers.push_back(std::move(dna));
    }
}

// converts a population to an atomarray
const c74::min::dict &Population::toDict() {

    t_dictionary *d = dictionary_new();
    long idx = 0;
    string basename = "pop_";

    for (auto pop : populationMembers) {
        string keyname = basename + to_string(idx++);
        dictionary_appendatomarray(d, gensym(keyname.c_str()),
                                   (t_object *)pop.toAtomArray());
    }
    dictionary_appendlong((c74::max::t_dictionary *)maxDict, gensym("generation"), generations);

    dictionary_appenddictionary((c74::max::t_dictionary *)maxDict, gensym("population"), (t_object *)d);

    return popDict;
}

// setter for the population size
void Population::setMaxPopulation(int mp) {
	Attributes::setMaxPopulation(mp); // call the base class implementation explicitly
    generations = 0;
    populationMembers.clear();
	for (int i = 0; i < getMaxPopulation(); i++) {
        DNA dna(numParams, true);
        populationMembers.push_back(std::move(dna));
    }
}

// uncomment to time the function, it's definitely faster
// #define BENCHMARK

// Compute average fitness for the population
double Population::getAverageFitness() {
    double total = 0.0;
    total = std::accumulate(LightIterator{populationMembers.begin()},
                            LightIterator{populationMembers.end()}, (double)0.0);

    return (total / (double)populationMembers.size());
}
// get the fittest member in the population.
// we use this to output the best member to Max
// get the fittest member in the population.
// we use this to output the best member to Max
vector<int> &Population::getBest(int *index) {
    
    static vector<int> defaultGenes = {-1};
    vector<int> diff;
    int idx = -1;
    for (int i = 0; i < populationMembers.size(); i++) {
        if (populationMembers[i].fitness > maxFitness) {
            maxFitness = populationMembers[i].fitness;
            idx = i;
        }
    }

    if (maxFitness >= getAccuracy()) {
        
        finished = true;
    }
    *index = idx;
    if (idx >= 0) {
    
        lastBest = populationMembers[idx].genes;
        return populationMembers[idx].genes;
    }
    return defaultGenes;
}


void Population::calcFitness() {
    if (targetParams.size()) {
        for (auto it = populationMembers.begin(); it < populationMembers.end(); it++) {
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

    std::sort(populationMembers.begin(), populationMembers.end(),
              [](const DNA &a, const DNA &b) -> bool {
                  return a.fitness > b.fitness;
              });
    int elitelen = populationMembers.size() * 0.1; //this takes care of preserving the best 10 percent of our old population. Its a sort of selection in itself.
    for (int i = 0; i < elitelen; i++) {
        newPopulation.push_back(populationMembers[i]);
    }

    exponentialRanker(getExpFactor());
    for (int i = 0; i < populationMembers.size(); i++) {
        sum += probabilityArray[i];
    }

    for (int i = 0; i < populationMembers.size() -elitelen ; i++) {
        DNA partnerA = rSelect();
        DNA partnerB = rSelect();
        DNA child = partnerA.crossover(
            partnerB); 
        child.mutate(getMutationRate(), getMutationIndex());
        newPopulation.push_back(std::move(child));
    }
    populationMembers.swap(newPopulation);
    generations++;

}

// Creates probability array which maps raw fitness values to values which have
// a hyper exponential curve.
// @Param c defines steepness of curve. where c must within the range [0..1).
// A small value of c increases the probability of the best phenotypes to be
// selected. If c is set to zero, the selection probability of the best phenotype
// is set to one. The selection probability of all other phenotypes is zero. A
// value near one equalizes the selection probabilities.

void Population::exponentialRanker(double c) {

    probabilityArray.clear();
    // first we need to sort the array in descending order
    // std::sort(population.begin(), population.end(), [](const DNA& a, const
    // DNA& b) -> bool { return a.fitness > b.fitness; });

    for (int i = 0; i < populationMembers.size(); i++) {

        double numerator = c - 1;
        double denominator = pow(c, populationMembers.size()) - 1.;
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

   
    return populationMembers[index];
}

DNA &Population::rSelect() {
    double rndNumber = rand() / (double)RAND_MAX;
    double offset = 0.0;
    int pick = 0;

    for (int i = 0; i < populationMembers.size(); i++) {
        offset += probabilityArray[i];
        if (rndNumber < offset) {
            pick = i;
            break;
        }
    }
    
    return populationMembers[pick];
}


int Population::getNumberOfParams() { return numParams; }


