/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.


#include "c74_min.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cstdlib>

using namespace std;

static double map(double value, int start1, double stop1,
                 int start2, int stop2) {
    double outgoing =
    start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
    
    return outgoing;
}

class DNA{
    
public:
    
    float fitness= 0;
    std::vector<int> genes;
    int numberOfGenes;
    //constructor for DNA class
    DNA(){
       
        this->numberOfGenes= 3; // TODO: change this to target.size()
        int randomGene= 0;
        for(int i = 0; i< numberOfGenes; i++){
            randomGene= rand() % 128;
            genes.push_back(randomGene);
        }
        
    }
    
    void fitnessFunction(vector<int> target){
        //our target will contain 3 elems to represent:
        // elem 0: osc pitch
        // elem 1: filter cutoff
        // elem 2: vca loudness
        int score = 0;
        for(int i = 0; i< genes.size(); i++){
            
            if(genes[i] == target[i]){
                score++;
            }
            // exponential fitting of score to fitness function to accentuate difference between a slightly better
            //result and its inferior
            fitness = pow(2, score);
        }
        
    }
    
    DNA crossover(DNA partner){
        
        DNA child;
        int midpoint = rand() % this->numberOfGenes;
        for(int i = 0; i< this->numberOfGenes; i++){
            if(i > midpoint){
                child.genes[i] = this->genes[i];
                
            }
            else{
              child.genes[i]= partner.genes[i];
            }
            
        }
        return child;
    }
    
    void mutate(float mutationRate){
        for(int i= 0; i< this->numberOfGenes; i++){
            double r = ((double) rand() / (RAND_MAX));
            if(r < mutationRate){
                this->genes[i] = rand() % 128;
            }
        }
        
    }
    
    void displayGenes(){
        for(auto it: genes){
            cout<< it << endl;
        }
    }


    
    
   
    
};



class Population{
public:
    
    vector<DNA> population;
    vector<DNA> matingPool;
    vector<int> targetParams;
    double mutationRate;
    int maxPopulation;
    
    int generations;              // Number of generations
    int finished;
    int perfectScore;
    
    //population can also serve as an interface
    //instead of using a constructor with 3 params
    //we call the default constructor and then
    //assign the attributes
    //target paramters, mutation rate, and max population number
    Population(){
        cout << "default constructor called" <<endl;
        targetParams= {12, 127, 38}; // find a better way to init these
        mutationRate= 0;
        maxPopulation= 1000;
        finished = false;
        //DNA population[maxPopulation];
        
        for(int i = 0; i< maxPopulation; i++){
            DNA dna;
            population.push_back(dna);
            
        }
        cout << "population size at the end of constructor" << population.size()<<endl;
            calcFitness();
    }
    
    void calcFitness(){
    
          
        for (int i = 0; i < population.size(); i++) {
       
            population[i].fitnessFunction(targetParams);
           
            
       }
    }
    // Compute average fitness for the population
     double getAverageFitness() {
       double total = 0;
       for (int i = 0; i < population.size(); i++) {
           total += population[i].fitness;
       }
         cout << "total is " << total<<endl;
       return total / (population.size());
     }
    
    // Generate a mating pool
    void naturalSelection() {
      // Clear the vector
        matingPool.clear();

        double maxFitness = 0;
        for (int i = 0; i < population.size(); i++) {
        if (population[i].fitness > maxFitness) {
          maxFitness = population[i].fitness;
        }
      }
    
    // Based on fitness, each member will get added to the mating pool a certain number of times
      // a higher fitness = more entries to mating pool = more likely to be picked as a parent
      // a lower fitness = fewer entries to mating pool = less likely to be picked as a parent
      for (int i = 0; i < population.size(); i++) {
        
        float fitness = map(population[i].fitness,0, maxFitness,0,1);
        int n = int(fitness * 100);  // Arbitrary multiplier, we can also use monte carlo method
        for (int j = 0; j < n; j++) {              // and pick two random numbers
          matingPool.push_back(population[i]);
        }
      }
    }
    
     // Compute the current "most fit" member of the population
        vector<int> getBest(){
      
         float maxFitness = 0.0;
         int index= 0;
         vector<int> best;
         for (int i = 0; i < population.size(); i++) {
         
             if (population[i].fitness > maxFitness) {
           maxFitness = population[i].fitness;
           index = i;
           }
           
         }
         if(maxFitness == perfectScore){
          finished = true;
         }
        return population[index].genes;

    }
    // Create a new generation
    void generate() {
      // Refill the population with children from the mating pool
      for (int i = 0; i < population.size(); i++) {
        int a = int(rand() % matingPool.size());
        int b = int(rand() % matingPool.size());
        DNA partnerA = matingPool[a];
        DNA partnerB = matingPool[b];
        DNA child = partnerA.crossover(partnerB);
        child.mutate(mutationRate);
        population[i] = child;
      }
      generations++;
    }
        
    
    bool terminate() {
       return finished;
     }

     int getGenerations() {
       return generations;
     }
    
};





using namespace c74::min;


class genetic : public object<genetic> {
public:
    
	MIN_DESCRIPTION {"apply genetic algorithm to n params"};
	MIN_TAGS {"time"};
	MIN_AUTHOR {"Cycling 74"};
	MIN_RELATED {"min.beat.pattern, link.beat, metro, tempo, drunk"};

   
  
	inlet<>  input {this, "(toggle) on/off"};
	outlet<> output {this, "(list) result of evolution"};
	
    Population population;
    
    
    attribute<vector<double>> target {this, "target", {1.0, 1.0 ,1.0},
		setter { MIN_FUNCTION {
			
            population = Population(); //reinitialize population to account for changing target
            
            population.targetParams.clear();
            for(auto it: args){
                        population.targetParams.push_back((int)it);
                           }
        
           
            return {args};
        }}};

    message<> bang {
    this, "bang", "test the functionality of DNA class.", MIN_FUNCTION {
        
        vector<int> currentBest;
        cout << "target params"<<c74::min::endl;

        for(auto it : population.targetParams){
               cout << it <<" " ;
               
           }
        cout <<c74::min::endl;
        
       
        if(!population.terminate()){
        // Generate mating pool
        population.naturalSelection();
        //Create next generation
        population.generate();
        // Calculate fitness
        population.calcFitness();
        currentBest= population.getBest();
       
        }
        cout << "current best: "<<c74::min::endl;
        
        atoms result;
        for(auto it : currentBest){
            cout << it <<" " ;
            result.push_back(it);
            
        }
        cout <<c74::min::endl;
        cout << "average: " <<population.getAverageFitness()<< c74::min::endl;

        //cast current best to atoms
        
        output.send(result);
        //        cout << "population itself" <<c74::min::endl;
//
//        for(int i = 0; i < population.population.size(); i++){
//            for(auto it : population.population[i].genes){
//        cout << it  << " " ;
//            }
//        cout <<c74::min::endl;
//        }
       
        return {};
    }};
        
       


};


MIN_EXTERNAL(genetic);


