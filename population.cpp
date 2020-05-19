#include "population.h"
#include <math.h>
#include <iostream>
#include <random>
#include <numeric>
using namespace std;


struct LightIterator : public std::vector<DNA>::iterator
{
    LightIterator(std::vector<DNA>::iterator it) : std::vector<DNA>::iterator(it) {}
    long double& operator*() { return std::vector<DNA>::iterator::operator*().fitness; }
};

//create a population with default values for mutation rate and population size.
//calculate the value of the perfect
//PARAMS: target params: input from max. For example a set of pixels
Population::Population(const vector<double>& tp){
    
    
    this->targetParams= tp;
    this->finished = false;
    this->population.clear();
    this->mutationRate= 0.07;
    this->perfectScore= pow((long double) 8, (long double) 5461);
    this->maxPopulation= 200;
    for(int i = 0; i< maxPopulation; i++){
        
        DNA dna(this->targetParams.size(), true);
        this->population.push_back(dna);
        
    }
    
    calcFitness();
}
   


//setter for the population size
void Population::setMaxPopulation(int mp){
    this->maxPopulation= mp;
    population.clear();
    for(int i = 0; i< this->maxPopulation; i++){
        DNA dna(this->targetParams.size(), true);
        population.push_back(dna);
    }
    calcFitness();
}
//Iterate through the population to calculate the fitness of each individual therein
void Population::calcFitness(){
    
    for (int i = 0; i < population.size(); i++) {
        
        population[i].fitnessFunction(targetParams);
        
    }
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
        DNA& partnerB = select(scores);
        DNA child = partnerA.crossover(partnerB);
        child.mutate(this->mutationRate, targetParams);
        newPopulation.push_back(child);
    }
    this->population.swap(newPopulation);
    //this->population= newPopulation;
    this->generations++;
    
    calcFitness();
    
}

//choose a single member of the population based on its score
DNA& Population::select(const vector<double>& scores){
    int index= 0;

    double random = this->equalRandom(gen);

    while(random > 0.0){
        random = random - scores[index];
        index++;
    }
    index--;


    population[index].count++;
    index= index % population.size();
    return population[index];
}
