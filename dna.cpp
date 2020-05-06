#include "dna.h"
#include <math.h>

using namespace std;

 //constructor for DNA class
 DNA::DNA(int paramSize){
     
     
     this->numberOfGenes= paramSize;
     int randomGene= 0;
     for(int i = 0; i< numberOfGenes; i++){
         randomGene= rand() % 256;
         genes.push_back(randomGene);
         
         
     }
 }
    void DNA::fitnessFunction(vector<int> target){
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
    
    void DNA::mutate(float mutationRate){
        for(int i= 0; i< this->numberOfGenes; i++){
            double r = ((double) rand() / (RAND_MAX));
            if(r < mutationRate){
                this->genes[i] = rand() % 256;
            }
        }
        
    }
    
//    void DNA::displayGenes(){
//        for(auto it: genes){
//            std::cout<< it << std::endl;
//        }
//    }
