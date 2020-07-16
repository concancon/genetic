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
    expFactor = 0.975;
    finished = false;
    population.clear();
    mutationRate = 0.001;
    maxPopulation = 11;
	numParams = numberOfParams;
    
    for(int i = 0; i < maxPopulation; i++) {
        DNA dna(numberOfParams, true);
        population.push_back(std::move(dna));
    }
    cout << "our new constructor has been called" <<endl;
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
    population.clear();
    for (int i = 0; i < maxPopulation; i++) {
        DNA dna(numParams, true);
        population.push_back(std::move(dna));
    }
    //calcFitness();
}

// uncomment to time the function, it's definitely faster
// #define BENCHMARK

//Iterate through the population to calculate the fitness of each individual therein
void Population::calcFitness(){

#ifdef BENCHMARK
	auto start = high_resolution_clock::now();
#endif

#if USE_THREADS
	int div = population.size() / numThreads;

	for (int i = 0; i < numThreads; i++) {
		workers[i]->doAsync([this, div, i] {  // add new tasks
			auto it1 = population.begin() + (i * div);
			auto it2 = (i == numThreads - 1) ? population.end() : (it1 + div); //if we are in the last thread then the end has to be rounded to end of the population.
			for ( ; it1 < it2; it1++) { //two iterators to address sub arrays of the population
				it1->fitnessFunction(targetParams);
			}
		});
	}
	for (int i = 0; i < numThreads; i++) {
		workers[i]->wait();  // wait for all threads to be finished. for synchronisation. pattern is called barrier.
	}
#else
	for (auto it = population.begin(); it < population.end(); it++) {
		it->fitnessFunction(targetParams);
	}
#endif

#ifdef BENCHMARK
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	std::cout << "calcFitness: " << duration.count() << std::endl;
#endif

}

// Compute average fitness for the population
double Population::getAverageFitness() {
    double total = 0.0;
    total = std::accumulate(LightIterator{population.begin()}, LightIterator{population.end()}, (double)0.0);
        
    return (total / (double)population.size());
}
//get the fittest member in the population.
//we use this to output the best member to Max
vector<int>& Population::getBest(int& index) {

    static vector<int> defaultGenes = {-1};

	maxFitness = 0.;
	index= -1;
    for (int i = 0; i < population.size(); i++) {
        if (population[i].fitness > maxFitness) {
            maxFitness = population[i].fitness;
            index = i;
        }
    }
  
    if (maxFitness >= numParams * 0.95) {
        finished = true;
    }
	if (index >= 0) {
		return population[index].genes;
	}
	return defaultGenes;
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
        DNA partnerA = select(sum);
        DNA partnerB = select(sum);
        DNA child = partnerA.crossover(partnerB); // this should be moved or elided, thus ok
        child.mutate(mutationRate, mutationIndex);
        newPopulation.push_back(std::move(child)); //std::move(child));
    }
    population.swap(newPopulation);
    //population= newPopulation;
    generations++;
    
   // calcFitness();
}


std::vector<double>& Population::exponentialRankSelector(double c){
    
    probabilityArray.clear();
     //first we need to sort the array in descending order
    //std::sort(population.begin(), population.end(), [](const DNA& a, const DNA& b) -> bool { return a.fitness > b.fitness; });

     for(int i = 0; i< population.size(); i++){
        
         double nominator = pow(c, (i-1.));
         double denominator = pow(c, population.size()) -1.;
         double probability = (c - 1.) * (nominator/ denominator);
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




vector<double>& Population::displayPopulation() {
	for (int i = 0; i < population.size(); i++) {
        for (int j = 0; j < population[i].genes.size(); j++) {
            //count the occurence of each gene
            counter[population[i].genes[j]]++;
        }
    }
    return counter;
}
