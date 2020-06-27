#include <math.h>
#include <iostream>
#include "dna.h"
#include "util.h"

using namespace std;

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

void DNA::fitnessFunction(const vector<double>& target){
     
     double score = 0;
     
     for(int i = 0; i< numberOfGenes; i++){
    
        int difference= abs (target[i] - genes[i]);
         //if difference > 50: threshold
         
         double distance = (double) (255.0 - difference) / 255.0;
         score+= distance;
         
     }
         
     score = utilities::map(score, 0, genes.size(), 0.0, 5461);
     // exponential fitting of score to fitness function to accentuate difference between a slightly better
              //result and its inferior
     this->fitness = pow((long double) 8.0, (long double) score);

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
void DNA::mutate(double mutationRate, const vector<double>& target){
    for(int i= 0; i< this->numberOfGenes; i++){
        double r = ((double) rand() / (RAND_MAX));
       // && this->genes[i] != target[i]
        if(r < mutationRate ){
            this->genes[i] = equalRandom(gen);
        }
    }
    
}
    
void DNA::displayGenes() {
	for (auto it: genes) {
		std::cout << it << " ";
	}
	cout << endl;
}
