#include <math.h>
#include <iostream>
#include <random>
#include <numeric>
#include <chrono>
#include "population.h"

using namespace std;
using namespace std::chrono;
using namespace c74::max;
struct LightIterator : public std::vector<DNA>::iterator
{
    LightIterator(std::vector<DNA>::iterator it) : std::vector<DNA>::iterator(it) {}
    double& operator*() { return std::vector<DNA>::iterator::operator*().fitness; }
};

//create a population with default values for mutation rate and population size.
//calculate the value of the perfect
//PARAMS: target params: input from max. For example a set of pixels
Population::Population(const vector<double>& tp): popDict(c74::min::symbol(true)), counter(256) {
    
    maxDict = (t_object*)popDict; //produces a memory leak //TODO: CREATE DESTRUCTOR TO RELEASE THIS
    expFactor = 0.975;
    targetParams= tp;
    finished = false;
    population.clear();
    mutationRate = 0.001;
    maxPopulation = 6;
	numParams = targetParams.size();
    
    for(int i = 0; i< maxPopulation; i++) {
        DNA dna(numParams, true);
        population.push_back(std::move(dna));
    }

#if USE_THREADS
	for (int i = 0; i < numThreads; i++) {
		workers[i] = std::make_unique<WorkerThread>();
	}
#endif
    cout << "our old constructor has been called" <<endl;
}

Population::Population(int numberOfParams): popDict(c74::min::symbol(true)), counter(256) {
    
    maxDict = (t_object*)popDict; //produces a memory leak //TODO: CREATE DESTRUCTOR TO RELEASE THIS
    expFactor = 0.123;
    finished = false;
    population.clear();
    mutationRate = 0.214;
    mutationIndex= 5.;
    maxPopulation = 50;
    accuracy = 97;
	numParams = numberOfParams;
    
    for(int i = 0; i < maxPopulation; i++) {
        DNA dna(numberOfParams, true);
        population.push_back(std::move(dna));
    }
}
   
//converts a population to an atomarray
const c74::min::dict& Population::toDict(){

	t_dictionary* d = dictionary_new();
    long idx = 0;
	string basename = "pop_";

    for(auto pop : population) {
		string keyname = basename + to_string(idx++);
		dictionary_appendatomarray(d, gensym(keyname.c_str()), (t_object*)pop.toAtomArray());
	}
    dictionary_appendlong(maxDict, gensym("generation"), generations);
    
    dictionary_appenddictionary(maxDict, gensym("population"), (t_object *)d);
    
	return popDict;
}

//setter for the population size
void Population::setMaxPopulation(int mp){
    maxPopulation = mp;
    generations = 0;
    population.clear();
    for (int i = 0; i < maxPopulation; i++) {
        DNA dna(numParams, true);
        population.push_back(std::move(dna));
    }
    //calcFitness();
}

void Population::setMutationRate(double mr){
    
    mutationRate = mr;
    
}
void Population::setExpFactor(double ef){
    
    expFactor= ef;
    
}

void Population::setAccuracy(double a){
    accuracy= a;
}
// uncomment to time the function, it's definitely faster
// #define BENCHMARK


// Compute average fitness for the population
double Population::getAverageFitness() {
    double total = 0.0;
    total = std::accumulate(LightIterator{population.begin()}, LightIterator{population.end()}, (double)0.0);
        
    return (total / (double)population.size());
}
//get the fittest member in the population.
//we use this to output the best member to Max
vector<int>& Population::getBest(int& index) {

//    cout << "max pop: " << maxPopulation <<endl;
//    cout << "accuracy"   << accuracy << endl;
    static vector<int> defaultGenes = {-1};

	maxFitness = 0.;
	index= -1;
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
		return population[index].genes;
	}
	return defaultGenes;
}


void Population::calcFitness(){
    if(targetParams.size()){
        for (auto it = population.begin(); it < population.end(); it++) {
            it->fitnessFunction(targetParams);
        }
    }
}
//This method replaces what used to be the natural selection method.
//it is based on the idea of setting each fitness value to a percentage in
//relation to the sum of all individuals.
//This percentage is then used when selecting a score, whereby a member with a higher score has a greater
//probability of being chosen.
void Population::generate(double mutationIndex) {
    
    double sum = 0.;
    // Refill the population with children from the mating pool
    newPopulation.clear();
    
    std::sort(population.begin(), population.end(), [](const DNA& a, const DNA& b) -> bool { return a.fitness > b.fitness; });
    int elitelen = population.size() * 0.1;
    for (int i = 0; i < elitelen; i++) {
        newPopulation.push_back(population[i]);
    }

    exponentialRankSelector(expFactor);
    for (int i = 0; i<population.size(); i++){
        sum+=probabilityArray[i];
    }
  
    for (int i = 0; i < population.size() - elitelen; i++) {
        DNA partnerA = rSelect();
        DNA partnerB = rSelect();
        DNA child = partnerA.crossover(partnerB); // this should be moved or elided, thus ok
        child.mutate(mutationRate, mutationIndex);
        newPopulation.push_back(std::move(child)); //std::move(child));
    }
    population.swap(newPopulation);
    //population= newPopulation;
    generations++;
    
   // calcFitness();
}

//Creates probability array which maps raw fitness values to values which have a hyper exponential curve.
// @Param c defines steepness of curve. where c must within the range [0..1).
//A small value of c increases the probability of the best phenotypes to be selected. If c is set to zero, the selection probability of the best
//phenotype is set to one. The selection probability of all other phenotypes is zero. A value near one equalizes the selection probabilities.

std::vector<double>& Population::exponentialRankSelector(double c){
    
    probabilityArray.clear();
     //first we need to sort the array in descending order
    //std::sort(population.begin(), population.end(), [](const DNA& a, const DNA& b) -> bool { return a.fitness > b.fitness; });

     for(int i = 0; i< population.size(); i++){
        
         double numerator = c-1;
         double denominator=  pow(c, population.size()) -1.;
         double probability = pow(c, i) *  (numerator/ denominator);
         probabilityArray.push_back(probability);
     }
     return probabilityArray;
 }
//select based on exponentialSelector
DNA& Population::select(double sum) {
    
    double random = (double)equalRandom(gen) * sum ;

    int index = 0;
    for ( ; random > 0. && index <probabilityArray.size(); index++) {
        random -= probabilityArray[index];
    }
    if (random > 0.0) {
        cout << "random is greater than 0!" << endl;
        
    }
    index--;

    population[index].count++;
    return population[index];
    
    
}

DNA& Population::rSelect(){
    double rndNumber = rand() / (double) RAND_MAX;
    double offset = 0.0;
    int pick = 0;

    for (int i = 0; i < population.size(); i++) {
        offset += probabilityArray[i];
        if (rndNumber < offset) {
            pick = i;
            break;
        }
        
    }
    population[pick].count++;
    return population[pick];
   
    
}

vector<double>& Population::displayPopulation() {
	for (int i = 0; i < population.size(); i++) {
        for (int j = 0; j < population[i].genes.size(); j++) {
            //count the occurence of each gene
            counter[population[i].genes[j]]++;
        }
    }
    return counter;
}

int Population::getNumberOfParams(){
    return numParams;
}

vector<int> Population::getSelectionCount(){
    vector<int> numberOfTimesSelected;
    for(int i = 0; i <population.size(); i++){
        numberOfTimesSelected.push_back(population[i].count);
    }
        
    return numberOfTimesSelected;
}
