#include <math.h>
#include <iostream>
#include "dna.h"
#include "util.h"

using namespace std;

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen{rd()}; //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<double> equalRandom{0.0, 1.0};


 //constructor for DNA class
DNA::DNA(int paramSize, bool randomize)
	: genes(paramSize, 0)
	, numberOfGenes(paramSize)
{
	if (randomize) {
		for (int i = 0; i < numberOfGenes; i++) {
			int randomGene = (int)(equalRandom(gen) * 255);
			genes[i] = randomGene;
		}
	}
}

//qualify each indivudal based on the amount of genes that match our target
void DNA::fitnessFunction(const vector<double>& target){

	double score = 0;

	for (int i = 0; i < numberOfGenes; i++) {
		if (genes[i] == (int)target[i]){
			score++;         // <------ how can we avoid this loop?
		}
	}
	score = utilities::map(score, 0, genes.size(), 0.0, 5461);
	// exponential fitting of score to fitness function to accentuate difference between a slightly better
			 //result and its inferior
	fitness = pow((long double) 8, (long double) score);
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

//apply a random values to genes that DONT match the target value
void DNA::mutate(double mutationRate, const vector<double>& target) {
	for (int i = 0; i < numberOfGenes; i++) {
		double r = equalRandom(gen);
		if (r < mutationRate ) {
			genes[i] = (int)(equalRandom(gen) * 255);
		}
	}
}
    
void DNA::displayGenes() {
	for (auto it: genes) {
		std::cout << it << " ";
	}
	cout << endl;
}
