#pragma once

#include "c74_min_api.h"
#include <iostream>
#include <random>
#include <vector>

static std::random_device
    rad; // Will be used to obtain a seed for the random number engine
static std::mt19937 engine{
    rad()}; // Standard mersenne_twister_engine seeded with rd()
static std::uniform_real_distribution<double> drng{0., 1.};
static std::uniform_int_distribution<int> randomInt{0, 255};

class DNA {

  public:
    DNA(int paramSize, bool randomize);

    DNA(const std::vector<double> &tp);

    double fitness;

    std::vector<int> genes;

    int numberOfGenes;

    void setFitness(double f);

    void fitnessFunction(const std::vector<double> &target);

    DNA &crossover(const DNA &partner);
   
    c74::max::t_atomarray *toAtomArray();

    void mutate(double mutationRate, double eta);

    void polynomialMutationImpl(const std::pair<double, double> &bounds,
                                const double p_m, const double eta_m);

};
