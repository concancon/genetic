#pragma once

#include <random>
#include <iostream>
#include <thread>
#include "c74_min_api.h"
#include "dna.h"
#include "workerthread.h"
#define USE_THREADS 0

class Population{
public:

#if USE_THREADS
	const int numThreads = 4;
	std::unique_ptr<WorkerThread> workers[4];
#endif
   
    c74::min::dict popDict;
    c74::max::t_object* maxDict;   //we need a t_object to write to dictionary
	std::vector<DNA> population;
    std::vector<double> targetParams;
    std::vector<double> counter;
    std::vector<double> probabilityArray;
    double accuracy;
    double expFactor;
    double mutationIndex;
    double mutationRate;
    int maxPopulation;
    bool calledOnce= false;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen{rd()}; //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> equalRandom{0.0, 1.0};

    bool finished;
    double generations= 0;              // Number of generations
    double perfectScore;   
    double maxFitness=0.;
	int numParams = 0;

    std::vector<DNA> newPopulation;
    Population(const std::vector<double>& tp);
    Population(int numberOfParams);
    
    const c74::min::dict& toDict(); //TODO: MAKE PRIVATE
     
    void setMutationRate(double mr);
    void setMutationIndex(double mi){
        mutationIndex= mi;
    
    }
    double getMutationRate(){
        return mutationRate;
    }
    void setAccuracy(double a);
    void setExpFactor(double ef);
    void setMaxPopulation(int mp);
    int getMaxPopulation(){
        
        return this->maxPopulation;
    }
    
    double getMaxFitness(){
        return this->maxFitness;
    }
    void calcFitness();

    // Compute average fitness for the population
    double getAverageFitness();
    
     // Compute the current "most fit" member of the population
    std::vector<int>& getBest(int& index);
    
    // Create a new generation
    void generate(double eta);

    std::vector<double>& exponentialRankSelector(double c);
    
    DNA& select(double sum);
    DNA& rSelect();
    
    bool terminate() {
       return finished;
     }

    int getNumberOfParams();
    
    int getGenerations() {
       return generations;
     }
    
    std::vector<double>& displayPopulation();
    
};
