#include <math.h>
#include <iostream>
#include "dna.h"


using namespace std;
//using namespace c74::max;
std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen{rd()}; //Standard mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<int> equalRandom{0, 255};


 //constructor for DNA class
DNA::DNA(int paramSize, bool randomize)
	: genes(paramSize, 0)
	, numberOfGenes(paramSize)
{
    int randomGene = 0;
	if (randomize) {
		for (int i = 0; i < numberOfGenes; i++) {
			randomGene= equalRandom(gen);
			genes[i] = randomGene;
		}
	}
}

//c74::max::t_atomarray* DNA::toAtomArray(){
//    
//    long ac = numberOfGenes;
//    t_atom* av = (t_atom*)sysmem_newptr(sizeof(t_atom)* ac);
//    long idx = 0;
//    
//    for(auto gene : genes){
// 
//        atom_setlong(av+idx++, gene);
//        
//        
//    }
//    
//    t_atomarray* aa = atomarray_new(ac, av);
//    sysmem_freeptr(av);
//    return aa;
//    
//}


void DNA::fitnessFunction(const vector<double>& target){
     
     
     double score = 0.;
     
     for(int i = 0; i< numberOfGenes; i++){
    
        int difference= abs (target[i] - genes[i]);
       
        double distance = (double) (255.0 - difference) / 255.0;
        score+= distance;
         
     }
    

    
    this->fitness = score; //TODO: GET RID OF SCORE VAR IF THIS APPROACH WORKS. Currently range is 0-1600 or 0-number of genes  
 }
    //combine two DNA's to generate a third. This is done stochastically
DNA& DNA::crossover(const DNA& partner) {
	//DNA child(numberOfGenes, false);
	if (numberOfGenes != 0) {
		int midpoint =  (int)(equalRandom(gen) * (double)numberOfGenes);
		for (int i = 0; i < numberOfGenes; i++) {
			if (i > midpoint) {
                ;
			}
			else {
				genes[i] = partner.genes[i];
			}
		}
	}
	return *this;
}

//apply a random values to random genes that DONT match the target value
void DNA::mutate(double mutationRate, double eta){
   
    for(int i= 0; i< this->numberOfGenes; i++){
        double r = ((double) rand() / (RAND_MAX));
      
        if(r < mutationRate ){
            //this->genes[i] = equalRandom(gen);
            this->genes[i] = polynomialMutate(this->genes[i], eta); //TODO: work here with dnas instead of a single value. just pass mutationRate and eta here as well
        }
    }
    
}


void DNA::displayGenes() {
	for (auto it: genes) {
		std::cout << it << " ";
	}
	cout << endl;
}
