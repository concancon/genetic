#pragma once

#include <vector>
#include <random>
#include <iostream>
#include "c74_min.h"


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
    c74::max::t_atomarray* toAtomArray();
    void mutate(double mutationRate, double eta);
    
  
    int static polynomialMutate(int originalValue, double index){
       
        //calculate L: the total number of bits used to represent all n variables
        
        //first we get u: a random number between 0 and 1
        double u = equal(engine);
        //calculate parameters dsubl and dsubr
        double newValue= 0;
        
        
        //dsubl
        if(u<= 0.5){
            
            double sublExponent= 1./(1.+ index);
            double dsubl= (pow((2.*u), sublExponent)) - 1.;
            newValue = originalValue + dsubl * (originalValue - 1.); //20 is assumed to be the lower bound a.k.a x sub i ° l
            newValue = floor(newValue);
        }
        
        else{


        double subrExponent = 1./(1. + index);
        double dsubr= 1. - pow((2.*(1.-u)), subrExponent);
        newValue = originalValue + dsubr*(255. - originalValue); //100 is assumed to be the upper bound a.k.a x sub i ^ u
        newValue = floor(newValue);
        }
        
        return newValue;
    }
        
    
    void displayGenes();
};


