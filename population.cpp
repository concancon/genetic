#include "population.h"
#include <math.h>
#include <iostream>
#include <random>
#include <numeric>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct LightIterator : public std::vector<DNA>::iterator
{
    LightIterator(std::vector<DNA>::iterator it) : std::vector<DNA>::iterator(it) {}
    long double& operator*() { return std::vector<DNA>::iterator::operator*().fitness; }
};

//create a population with default values for mutation rate and population size.
//calculate the value of the perfect
//PARAMS: target params: input from max. For example a set of pixels
Population::Population(const vector<double>& tp): counter(256){
    
    
    targetParams= tp;
    finished = false;
    population.clear();
    mutationRate= 0.07;
    perfectScore= pow((long double) 8, (long double) 5461);
    maxPopulation= 200;
    for(int i = 0; i< maxPopulation; i++) {
        DNA dna(targetParams.size(), true);
        population.push_back(std::move(dna));
    }

#if USE_THREADS
	for (int i = 0; i < numThreads; i++) {
		workers[i] = std::make_unique<WorkerThread>();
	}
#endif
    calcFitness();
}
   
//setter for the population size
void Population::setMaxPopulation(int mp){
    maxPopulation= mp;
    population.clear();
    for(int i = 0; i< maxPopulation; i++){
        DNA dna(targetParams.size(), true);
        population.push_back(std::move(dna));
    }
    calcFitness();
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
     total = std::accumulate(LightIterator{population.begin()}, LightIterator{population.end()}, (long double) 0.0);
    return (total / double(population.size()));
}

//get the fittest member in the population.
//we use this to output the best member to Max
vector<int> Population::getBest(){
    
    
    int index= 0;
    for (int i = 0; i < population.size(); i++) {
        
        if (population[i].fitness > maxFitness) {
            maxFitness = population[i].fitness;
            index = i;
        }
        
    }
    //cout << "Perfect score: " << perfectScore<< endl;
    //cout << "Max Fitness: " << maxFitness<<endl;
    if(maxFitness == perfectScore){
        finished = true;
    }
    return population[index].genes;
    
}

//This method replaces what used to be the natural selection method.
//it is based on the idea of setting each fitness value to a percentage in
//relation to the sum of all individuals.
//This percentage is then used when selecting a score, whereby a member with a higher score has a greater
//probability of being chosen.
void Population::generate() {
    
    // Refill the population with children from the mating pool
    newPopulation.clear();
    //newPopulation.shrink_to_fit();
    vector<double> scores(population.size());
    
    long double sum = 0.0;

    sum = std::accumulate(LightIterator{population.begin()}, LightIterator{population.end()}, (long double) 0.0);
    long double inverseSum = 1.0/ sum;
    for (int i = 0; i < population.size(); i++) {
        
       scores[i]= population[i].fitness* inverseSum;
        
    }
    
    for (int i = 0; i < population.size(); i++) {
        
        DNA& partnerA = select(scores);
        //DNA p = partnerA;
        DNA& partnerB = select(scores);
		DNA child = partnerA.crossover(partnerB); // this should be moved or elided, thus ok
        child.mutate(mutationRate, targetParams);
		newPopulation.push_back(child); //std::move(child));
    }
    population.swap(newPopulation);
    //population= newPopulation;
    generations++;
    
    calcFitness();
    
}

//choose a single member of the population based on its score
DNA& Population::select(const vector<double>& scores){
    int index= 0;

    double random = equalRandom(gen);

    while(random > 0.0 && index < scores.size()){
        random = random - scores[index];
        index++;
    }
    if(random >  0.0){
        cout<< "random is greater than 0!" <<endl;
    }
    index--;

    
    //index= index % population.size();
    population[index].count++;
    return population[index];
}

vector<double>& Population::displayPopulation() {
	for(int i = 0; i < population.size(); i++) {
        for (int j = 0; j < population[i].genes.size(); j++) {
            //count the occurence of each gene
            counter[population[i].genes[j]]++;
        }
    }
    return counter;
}
