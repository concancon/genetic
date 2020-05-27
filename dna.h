#pragma once

#include <vector>
#include <random>
#include <iostream>
class DNA{
    
public:
    
    DNA(int paramSize, bool randomize);
    long double fitness;
    std::vector<int> genes;
    int numberOfGenes;
    int count = 0;
    void fitnessFunction(const std::vector<double>& target);
   
    
    // exponential fitting of score to fitness function to accentuate difference between a slightly better
    //result and its inferior
    void crossover(const DNA& partner, DNA& child);
    
    void mutate(double mutationRate, const std::vector<double>& target);
    
    void displayGenes();
};


