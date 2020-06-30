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
    DNA& crossover(const DNA& partner);
    
    void mutate(double mutationRate);
    
    void displayGenes();
};


