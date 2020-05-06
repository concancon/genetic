#pragma once

#include "dna.h"



class Population{
public:
    
    std::vector<DNA> population;
    std::vector<DNA> matingPool;
    std::vector<int> targetParams;
    double mutationRate;
    int maxPopulation;
    bool calledOnce= false;
    
    int generations;              // Number of generations
    int finished;
    int perfectScore;
    std::vector<DNA> newPopulation;
    //population can also serve as an interface
    //instead of using a constructor with 3 params
    //we call the default constructor and then
    //assign the attributes
    //target parameters, mutation rate, and max population number
    Population();
    Population(std::vector<int> tp);
    
    void setMutationRate(double mr){
        this->mutationRate= mr;
    }
    double getMutationRate(){
        return this->mutationRate;
    }
    void setMaxPopulation(int mp);
    int getMaxPopulation(){
        
        return this->maxPopulation;
    }
    
    void calcFitness();

    // Compute average fitness for the population
    double getAverageFitness();
    
     // Compute the current "most fit" member of the population
    std::vector<int> getBest();
    
    // Create a new generation
    void generate();

    DNA select(std::vector<double> scores);
    
    bool terminate() {
       return finished;
     }

     int getGenerations() {
       return generations;
     }
    
};
