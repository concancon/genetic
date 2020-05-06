#pragma once

#include <vector>

class DNA{
    
public:
    
    DNA(int paramSize);
    float fitness;
    std::vector<int> genes;
    int numberOfGenes;
    
    void fitnessFunction(std::vector<int> target);
    
    // exponential fitting of score to fitness function to accentuate difference between a slightly better
    //result and its inferior
    DNA crossover(DNA partner);
    
    void mutate(float mutationRate);
    
    void displayGenes();
};


