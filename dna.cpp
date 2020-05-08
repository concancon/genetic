#include "dna.h"
#include <math.h>
#include <iostream>
using namespace std;

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen{rd()}; //Standard mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<int> equalRandom{0, 255};


 //constructor for DNA class
 DNA::DNA(int paramSize){
     
     
     this->numberOfGenes= paramSize;
     int randomGene= 0;
     for(int i = 0; i< numberOfGenes; i++){
         randomGene= equalRandom(gen);
         genes.push_back(randomGene);
         
         
     }
 }
    void DNA::fitnessFunction(vector<double> target){
        
        int score = 0;
        
        for(int i = 0; i< genes.size(); i++){
       
            if(genes[i] == target[i]){
                score++;
            }
        }
        
        
       
        // exponential fitting of score to fitness function to accentuate difference between a slightly better
                 //result and its inferior
        this->fitness = pow(2, score);
     

    }
  
    
    DNA DNA::crossover(DNA partner){
        
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
    
    void DNA::mutate(double mutationRate, vector<double> target){
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
