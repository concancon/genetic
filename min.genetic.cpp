/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.
// code based on Dan Schiffman's GA Tutorial from NOC: https://natureofcode.com/book/chapter-9-the-evolution-of-code/

#include "c74_min.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cstdlib>

using namespace std;

//static double map(double value, int start1, double stop1,
//                 int start2, int stop2) {
//    double outgoing =
//    start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
//
//    return outgoing;
//}

class DNA{
    
public:
    
    float fitness= 0;
    std::vector<int> genes;
    int numberOfGenes;
    //constructor for DNA class
 
    DNA(int paramSize){
       
        this->numberOfGenes= paramSize; // TODO: change this to target.size()
        int randomGene= 0;
        for(int i = 0; i< numberOfGenes; i++){
            randomGene= rand() % 256;
            genes.push_back(randomGene);
        }
        
    }
//
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
        }
       
        // exponential fitting of score to fitness function to accentuate difference between a slightly better
                 //result and its inferior
        fitness = pow(12, score);

    }
  
    
    DNA crossover(DNA partner){
        
        DNA child(this->numberOfGenes);
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
                this->genes[i] = rand() % 256;
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
    bool calledOnce= false;
    
    int generations;              // Number of generations
    int finished;
    int perfectScore;
    vector<DNA> newPopulation;
    //population can also serve as an interface
    //instead of using a constructor with 3 params
    //we call the default constructor and then
    //assign the attributes
    //target parameters, mutation rate, and max population number
    Population(){
       
        //cout << "default constructor called" <<endl;
        targetParams= {2, 2, 3}; // find a better way to init these
        mutationRate= 0;
        finished = false;

        
        for(int i = 0; i< maxPopulation; i++){
           
            DNA dna(this->targetParams.size());
            population.push_back(dna);
            
        }
    
            calcFitness();
    }
     Population(vector<int> tp){
        this->targetParams= tp;
        this->mutationRate= 0;
        this->maxPopulation= 100;
        this->finished = false;
        this->population.clear();
        this->perfectScore= pow(12, targetParams.size());
        for(int i = 0; i< maxPopulation; i++){
               
                DNA dna(this->targetParams.size());
                this->population.push_back(dna);
                
            }
       
            this->calcFitness();
        
    }
    
    void setMutationRate(double mr){
        this->mutationRate= mr;
    }
    double getMutationRate(){
        return this->mutationRate;
    }
    void setMaxPopulation(int mp){
        if(mp)this->maxPopulation= mp;
        population.clear();
        for(int i = 0; i< this->maxPopulation; i++){
                 DNA dna(this->targetParams.size());
                 population.push_back(dna);
             }
        calcFitness();
    }
    
    int getMaxPopulation(){
        
        return this->maxPopulation;
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
         //cout << "total is " << total<<endl;
       return total / (population.size());
     }
    
     // Compute the current "most fit" member of the population
        vector<int> getBest(){
      
         float maxFitness = 0.0;
         int index= 0;
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
        if(!this->finished){
            // Refill the population with children from the mating pool
            newPopulation.clear();
            vector<double> scores(population.size());
            double sum = 0.0;
            for (int i = 0; i < population.size(); i++) {
                sum += population[i].fitness;
            }
            // cout<< "population fitness sum is: " << sum<<endl<<endl;
            for (int i = 0; i < population.size(); i++) {
                
                scores[i]= population[i].fitness/ sum;
                
            }
            
            for (int i = 0; i < population.size(); i++) {
                
                DNA partnerA = select(scores);
                DNA partnerB = select(scores);
                DNA child = partnerA.crossover(partnerB);
                child.mutate(mutationRate);
                newPopulation.push_back(child);
            }
            this->population= newPopulation;
            generations++;
        
            //cout<< "average fitness: " << getAverageFitness() << c74::min::endl;
        }
        else{
            cout << "ALREADY FINISHED!" <<endl;
        }
    }

  
    DNA select(vector<double> scores){
        int index= 1;
        double random = (rand() % 10) / 10.0;
        cout << "random: " << random <<endl;
        while(random > 0.0){
            random = random - scores[index];
            index++;
        }
        index--;
    
        cout << "selected index was: " <<index<<endl;
        
        return population[index];
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

private:
    Population population;
    vector<int> currentBest;
    atoms result;
public:
    
	MIN_DESCRIPTION {"apply genetic algorithm to n params"};
	MIN_TAGS {"time"};
	MIN_AUTHOR {"Cycling 74"};
	MIN_RELATED {""};

   
  
	inlet<>  input {this, "(toggle) on/off"};
	outlet<> output {this, "(list) result of evolution"};
	
    
   
    attribute<vector<double>> target {this, "target", {1, 1 ,1, 1},
		setter { MIN_FUNCTION {
            vector<int> sVec;
            population.targetParams.clear();
            for(auto it: args){
                       sVec.push_back((int)it);
                           }
            population= Population(sVec);
        
            return {args};
        }}};
    
    attribute<double> mutationRate {this, "mutationRate", 0.01,
        setter { MIN_FUNCTION {
            
            population.setMutationRate(double(args[0]));

            return {args};
        }}};
            
            
    attribute<int> maxPopulation {this, "maxPopulation", 100,
            setter { MIN_FUNCTION {

            //cout << "args[0] " << int(args[0]) << c74::min::endl;
                population.setMaxPopulation(int(args[0]));
                return {args};
            }}};


    message<> bang {
    this, "bang", "test the functionality of DNA class.", MIN_FUNCTION {
    
        //cout <<c74::min::endl;
        currentBest.clear();
        result.clear();
       
        if(!population.terminate()){
        
        // Generate mating pool
        //population.naturalSelection();
   
        //Create next generation
        population.generate();
        // Calculate fitness
        population.calcFitness();

        currentBest= population.getBest();
       
        }
        for(auto it : currentBest){
            result.push_back(it);
            
        }
  

        //cast current best to atoms
        
        output.send(result);
       
        return {};
    }};
        
       


};


MIN_EXTERNAL(genetic);


