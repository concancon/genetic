#include "population.h"
#include <math.h>
#include <iostream>

using namespace std;
Population::Population(){
   
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
 Population::Population(vector<int> tp){
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


void Population::setMaxPopulation(int mp){
    if(mp)this->maxPopulation= mp;
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
  double total = 0;
  for (int i = 0; i < population.size(); i++) {
      total += population[i].fitness;
  }
    //cout << "total is " << total<<endl;
  return total / (population.size());
}

vector<int> Population::getBest(){
     
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

void Population::generate() {
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
      
          cout<< "average fitness: " << getAverageFitness() << endl;
      }
      else{
          //cout << "ALREADY FINISHED!" <<endl;
      }
  }


  DNA Population::select(vector<double> scores){
      int index= 1;
      double random = (rand() % 10) / 10.0;
      //cout << "random: " << random <<endl;
      while(random > 0.0){
          random = random - scores[index];
          index++;
      }
      index--;
  
      //cout << "selected index was: " <<index<<endl;
      
      return population[index];
  }
