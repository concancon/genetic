#pragma once

#include "dna.h"
#include <random>
#include <iostream>
#include <thread>
#include "workerthread.h"
#define USE_THREADS 1

class Population{
public:

#if USE_THREADS
	const int numThreads = 4;
	std::unique_ptr<WorkerThread> workers[4];
#endif

	std::vector<DNA> population;
    std::vector<DNA> matingPool;
    std::vector<double> scores;
    std::vector<double> targetParams;
    std::vector<double> counter;
    
    double mutationIndex= 0.;
    int maxPopulation;
    bool calledOnce= false;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen{rd()}; //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> equalRandom{0.0, 1.0};
    double mutationRate;
    bool finished;
    double generations= 0;              // Number of generations
    long double perfectScore;
    long double maxFitness=0.;

    std::vector<DNA> newPopulation;
    Population(const std::vector<double>& tp);
    
    void setMutationRate(double mr){
       mutationRate= mr;
    }
    
    void setMutationIndex(double mi){
        mutationIndex= mi;
    
    }
    double getMutationRate(){
        return mutationRate;
    }
    void setMaxPopulation(int mp);
    int getMaxPopulation(){
        
        return this->maxPopulation;
    }
    
    long double getMaxFitness(){
        return this->maxFitness;
    }
    void calcFitness();

    // Compute average fitness for the population
    double getAverageFitness();
    
     // Compute the current "most fit" member of the population
    std::vector<int>& getBest(int& index);
    
    // Create a new generation
    void generate(double eta);

    DNA& select(const std::vector<long double>& scores, long double sum);
    
    bool terminate() {
       return finished;
     }

     int getGenerations() {
       return generations;
     }
    
    std::vector<double>& displayPopulation();
    
};
