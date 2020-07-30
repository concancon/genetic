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
            polynomialMutationImpl({0.,255.}, mutationRate, eta);
        }
    }
    
}

//dont pass child as reference, just use the class we're in and mutate 'in place' :)
void DNA::polynomialMutationImpl(const std::pair<double, double> &bounds,
                                     const double p_m, const double eta_m){
    
   std::random_device rd;  //Will be used to obtain a seed for the random number engine
   std::mt19937 randomEngine{rd()}; //Standard mersenne_twister_engine seeded with rd()//TODO: MAKE THESE GLOBAL
   //child.genes[0] = 200;
   // Decision vector dimensions
   auto nx = this->genes.size();
   auto ncx = nx;
   // Problem bounds
   const auto lb = bounds.first;
   const auto ub = bounds.second;
   // declarations
   double rnd, delta1, delta2, mut_pow, deltaq;
   double y, yl, yu, val, xy;
   // Random distributions
   std::uniform_real_distribution<> drng(0., 1.); // to generate a number in [0, 1)
   // This implements the real polinomial mutation and applies it to the non integer part of the decision vector
   for (decltype(ncx) j = 0u; j < ncx; ++j) {
       if (drng(randomEngine) < p_m && lb != ub) {
           y = this->genes[j];
           yl = lb;
           yu = ub;
           delta1 = (y - yl) / (yu - yl);
           delta2 = (yu - y) / (yu - yl);
           rnd = drng(randomEngine);
           
           mut_pow = 1. / (eta_m + 1.);
           if (rnd < 0.5) {
               xy = 1. - delta1;
               val = 2. * rnd + (1. - 2. * rnd) * (std::pow(xy, (eta_m + 1.)));
               deltaq = std::pow(val, mut_pow) - 1.;
           } else {
               xy = 1. - delta2;
               val = 2. * (1. - rnd) + 2. * (rnd - 0.5) * (std::pow(xy, (eta_m + 1.)));
               deltaq = 1. - (std::pow(val, mut_pow));
           }
           y = y + deltaq * (yu - yl);
           if (y < yl) y = yl;
           if (y > yu) y = yu;
           this->genes[j] = y;
       }
   }

   // This implements the integer mutation for an individual
   for (decltype(nx) j = ncx; j < nx; ++j) {
       if (drng(randomEngine) < p_m) {
           // We need to draw a random integer in [lb, ub].
           rnd = drng(randomEngine);
           rnd *= (ub-lb);
           rnd += lb;
           //auto mutated = uniform_integral_from_range(lb, ub, randomEngine);
           this->genes[j] = rnd;
       }
   }
    
    
    
}



void DNA::displayGenes() {
	for (auto it: genes) {
		std::cout << it << " ";
	}
	cout << endl;
}
