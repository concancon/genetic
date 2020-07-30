#pragma once

#include <vector>
#include <random>
#include <iostream>
//#include "c74_min.h"


static std::random_device rad;  //Will be used to obtain a seed for the random number engine
static std::mt19937 engine{rad()}; //Standard mersenne_twister_engine seeded with rd()
static std::uniform_real_distribution<double> equal{0., 1.};

class DNA{
    
public:
    
    DNA(int paramSize, bool randomize);
    double fitness;
    std::vector<int> genes;
    int numberOfGenes;
    int count = 0;
    void fitnessFunction(const std::vector<double>& target);
 
    
    // exponential fitting of score to fitness function to accentuate difference between a slightly better
    //result and its inferior
    DNA& crossover(const DNA& partner);
   // c74::max::t_atomarray* toAtomArray();
    void mutate(double mutationRate, double eta);
    
  
    int static polynomialMutate(int originalValue , double index){
       
//        void static polynomial_mutation_impl( DNA& child, const std::pair<double, double> &bounds,
//        const double p_m, const double eta_m)
       
        
        DNA child(1, 0);
        child.genes[0] = originalValue;
        polynomial_mutation_impl(child, {0, 255}, 1., index);
        int result = child.genes[0];
        return result;
        
    }
    
    // Performs polynomial mutation. Requires all sizes to be consistent. Does not check if input is well formed.
    // p_m is the mutation probability, eta_m the distibution index
    void static polynomial_mutation_impl( DNA& child, const std::pair<double, double> &bounds,
                                   const double p_m, const double eta_m)
    {
        
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 randomEngine{rd()}; //Standard mersenne_twister_engine seeded with rd()//TODO: MAKE THESE GLOBAL
        
        //child.genes[0] = 200;
        // Decision vector dimensions
        auto nx = child.genes.size();
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
                y = child.genes[j];
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
                child.genes[j] = y;
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
                child.genes[j] = rnd;
            }
        }
    }
    
    
    
    
    
        
    
    void displayGenes();
};


