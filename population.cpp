#include "population.h"
#include <math.h>
#include <iostream>
#include <random>
using namespace std;

Population::Population(const vector<double>& tp){
    
    
    this->targetParams= tp;
    this->finished = false;
    this->population.clear();
    this->mutationRate= 0.07;
    this->perfectScore= pow(2, targetParams.size());
    this->maxPopulation= 1000;
    for(int i = 0; i< maxPopulation; i++){
        
        DNA dna(this->targetParams.size());
        this->population.push_back(dna);
        
    }
    
    calcFitness();
}
   



void Population::setMaxPopulation(int mp){
    this->maxPopulation= mp;
    population.clear();
    for(int i = 0; i< this->maxPopulation; i++){
        DNA dna(this->targetParams.size());
        population.push_back(dna);
    }
    calcFitness();
}

void Population::calcFitness(){
    
    for (int i = 0; i < population.size(); i++) {
        
        population[i].fitnessFunction(targetParams);
        
    }
}

// Compute average fitness for the population
double Population::getAverageFitness() {
    double total = 0.0;
    
    for (int i = 0; i < population.size(); i++) {
        total += population[i].fitness;
    }
    return (total / double(population.size()));
}

vector<int> Population::getBest(){
    
    
    int index= 0;
    for (int i = 0; i < population.size(); i++) {
        
        if (population[i].fitness > this->maxFitness) {
            this->maxFitness = population[i].fitness;
            index = i;
        }
        
    }
    //cout << "Perfect score: " << perfectScore<< endl;
    //cout << "Max Fitness: " << this->maxFitness<<endl;
    if(this->maxFitness == perfectScore){
        finished = true;
    }
    return population[index].genes;
    
}

void Population::generate() {
    
    // Refill the population with children from the mating pool
    newPopulation.clear();
    vector<double> scores(population.size());
    
    double sum = 0.0;
    for (int i = 0; i < population.size(); i++) {
        sum += population[i].fitness;
    }
    
    for (int i = 0; i < population.size(); i++) {
        
       scores[i]= population[i].fitness/ sum;
        
    }
    
    for (int i = 0; i < population.size(); i++) {
        
        DNA partnerA = select(scores);
        DNA partnerB = select(scores);
        DNA child = partnerA.crossover(partnerB);
        child.mutate(this->mutationRate, targetParams);
        newPopulation.push_back(child);
    }
    this->population= newPopulation;
    this->generations++;
    
    calcFitness();
    
}


DNA Population::select(const vector<double>& scores){
    int index= 0;
    
    double random = this->equalRandom(gen);
    
    while(random > 0.0){
        random = random - scores[index];
        index++;
    }
    index--;
    
    
    population[index].count++;
    return population[index];
}
