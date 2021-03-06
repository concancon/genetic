#include "dna.h"
#include <iostream>
#include <math.h>

using namespace std;
using namespace c74::max;

std::random_device
    rd; // Will be used to obtain a seed for the random number engine
std::mt19937 gen{rd()}; // Standard mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<int> equalRandom{0, 255};

void DNA::setFitness(double f) { fitness = f; }

// constructor for DNA class
DNA::DNA(int paramSize, bool randomize)
    : genes(paramSize, 0), numberOfGenes(paramSize) {
    if (randomize) {
        for (int i = 0; i < numberOfGenes; i++) {
            genes[i]=  randomInt(engine);
        }
    }
}
//overload constructor for testing
DNA::DNA(const vector<double> &tp): numberOfGenes(tp.size()){
        
    if(genes.size())genes.clear();
    for(auto it: tp){
        genes.push_back(it);
    }
    
}

//converts a dna's genes to an atom array
c74::max::t_atomarray *DNA::toAtomArray() {

    long ac = numberOfGenes;
    t_atom *av = (t_atom *)sysmem_newptr(sizeof(t_atom) * ac);
    long idx = 0;

    for (auto gene : genes) {
        atom_setlong(av + idx++, gene);
    }

    t_atomarray *aa = atomarray_new(ac, av);
    sysmem_freeptr(av);
    return aa;
}

//measures the fitness of an individual according to a target
void DNA::fitnessFunction(const vector<double> &target) {

    double score = 0.;

    for (int i = 0; i < numberOfGenes; i++) {

        int difference = abs(target[i] - genes[i]);

        score += (double)(255.0 - difference) / 255.0;
       
    }

    fitness = (score / numberOfGenes) *
              100.; // TODO: GET RID OF SCORE VAR IF THIS APPROACH WORKS.
                    // Currently range is 0-1600 or 0-number of genes
}
// combine two DNA's to generate a third. This is done stochastically
DNA &DNA::crossover(const DNA &partner) {
    // DNA child(numberOfGenes, false);
    if (numberOfGenes != 0) {
        int midpoint = (int)(drng(engine) * (double)numberOfGenes);
        
        for (int i = 0; i < numberOfGenes; i++) {
            if (i> midpoint) {
                genes[i] = partner.genes[i];
            }
            else if(i< midpoint){
                ; //leave as is
                
            }
            else if(i == midpoint){
                //this is happening a lot more often than expected
                //so we assume that we are going to stay with genes as is
                
                if((rand() % 2) + 1 == 2)genes[i] = partner.genes[i];
                
                
                
            }
        }
    }
    return *this;
}
// apply a random values to random genes that DONT match the target value
void DNA::mutate(double mutationRate, double eta) {

    polynomialMutationImpl({1., 255.}, mutationRate, eta);
}

// dont pass child as reference, just use the class we're in and mutate 'in
// place' :)
//induces an effect of a perturbation ofO((b−a)/ηm) in a variable, where a and b are lower and upper bounds of the variable. They also found that a value ηm ∈[20,100] is adequate in most problems that they tried.In this operator, a polynomial probability distribution is used to perturb a solution in a parent’s vicinity.The probability distribution in both left and right ofa variable value is adjusted so that no value outsidethe specified range [a, b] is created by the mutationoperato

void DNA::polynomialMutationImpl(const std::pair<double, double> &bounds,
                                 const double p_m, const double eta_m) {

    // child.genes[0] = 200;
    // Decision vector dimensions
    auto nx = genes.size();
    auto ncx = nx;
    // Problem bounds
    const auto lb = bounds.first;
    const auto ub = bounds.second;
    // declarations
    double rnd, delta1, delta2, mut_pow, deltaq;
    double y, yl, yu, val, xy;
    // Random distributions
    std::uniform_real_distribution<> drng(0.,
                                          1.); // to generate a number in [0, 1)
    // This implements the real polinomial mutation and applies it to the non
    // integer part of the decision vector
    for (decltype(ncx) j = 0u; j < ncx; ++j) {

        auto temp = drng(engine);
        if (temp < p_m && lb != ub) {
            y = genes[j];
            yl = lb;
            yu = ub;
            delta1 = (y - yl) / (yu - yl);
            delta2 = (yu - y) / (yu - yl);
            rnd = drng(engine);

            mut_pow = 1. / (eta_m + 1.);
            if (rnd < 0.5) {
                xy = 1. - delta1;
                val = 2. * rnd + (1. - 2. * rnd) * (std::pow(xy, (eta_m + 1.)));
                deltaq = std::pow(val, mut_pow) - 1.;
            } else {
                xy = 1. - delta2;
                val = 2. * (1. - rnd) +
                      2. * (rnd - 0.5) * (std::pow(xy, (eta_m + 1.)));
                deltaq = 1. - (std::pow(val, mut_pow));
            }
            y = y + deltaq * (yu - yl);
            if (y < yl)
                y = yl;
            if (y > yu)
                y = yu;
            genes[j] = y;
        }
    }

    // This implements the integer mutation for an individual
    for (decltype(nx) j = ncx; j < nx; ++j) {
        if (drng(engine) < p_m) {
            // We need to draw a random integer in [lb, ub].
            rnd = drng(engine);
            rnd *= (ub - lb);
            rnd += lb;
            // auto mutated = uniform_integral_from_range(lb, ub, randomEngine);
            genes[j] = rnd;
        }
    }
}


