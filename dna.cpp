#include <math.h>
#include <iostream>
#include "dna.h"
#include "util.h"

using namespace std;

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen{rd()}; //Standard mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<int> equalRandom{0, 255};


 //constructor for DNA class
 DNA::DNA(int paramSize, bool randomize): genes(paramSize){
     
     if(paramSize)this->numberOfGenes= paramSize;
     int randomGene= 0;
     if(randomize){
         for(int i = 0; i< numberOfGenes; i++){
             randomGene= equalRandom(gen);
             genes[i]= (randomGene);

         }
     }
 }
    void DNA::fitnessFunction(const vector<double>& target){
        
        double score = 0;
        
        for(int i = 0; i< numberOfGenes; i++){
       
            if(genes[i] == target[i]){
                score++;         // <------ how can we avoid this loop?
            }
        }
        
        score = utilities::map(score, 0, genes.size(), 0.0, 5461);
        // exponential fitting of score to fitness function to accentuate difference between a slightly better
                 //result and its inferior
        this->fitness = pow((long double) 8, (long double) score);

    }
  
    
    DNA DNA::crossover(const DNA& partner){
        
        DNA child(this->numberOfGenes, false);
        if(this->numberOfGenes!= 0){
        int midpoint = rand() % this->numberOfGenes;
        for(int i = 0; i< this->numberOfGenes; i++){
            if(i > midpoint){
               child.genes[i] = this->genes[i];
            }
            else{
              child.genes[i]= partner.genes[i];
            }
            
            }
        }
        return child;
    }
    
    void DNA::mutate(double mutationRate, const vector<double>& target){
        for(int i= 0; i< this->numberOfGenes; i++){
            double r = ((double) rand() / (RAND_MAX));
            
            if(r < mutationRate && this->genes[i] != target[i]){
                this->genes[i] = equalRandom(gen);
            }
        }
        
    }
    
    void DNA::displayGenes(){
        for(auto it: genes){
            std::cout<< it << " ";
        }
        cout << endl;
    }
