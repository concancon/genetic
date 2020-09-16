#pragma once

#include "c74_min_api.h"
#include "dna.h"
#include <iostream>
#include <random>
#include <thread>
#define DEFAULT_ACCURACY    (98.)
#define DEFAULT_MUTRATE     (0.214)
#define DEFAULT_MUTIDX      (5)
#define DEFAULT_MAXPOP      (10)
#define DEFAULT_EXPFACT     (0.123)

class Population {
  public:

    c74::min::dict popDict;
    c74::max::t_object *maxDict; // we need a t_object to write to dictionary
    std::vector<DNA> population;
    std::vector<double> targetParams;
    std::vector<double> probabilityArray;
    double accuracy;
    double expFactor;
    double mutationIndex;
    double mutationRate;
    int maxPopulation;
    bool calledOnce = false;
    std::random_device
        rd; // Will be used to obtain a seed for the random number engine
    std::mt19937 gen{rd()}; // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> equalRandom{0.0, 1.0};

    bool finished;
    double generations = 0; // Number of generations
    double perfectScore;
    double maxFitness = 0.;
    double rateOfImprovement = 0.;
    int numParams = 0;

    std::vector<DNA> newPopulation;
    std::vector<int> lastBest;
    Population(const std::vector<double> &tp);
    Population(int numberOfParams);

    void adoptAttributes(const Population &pop); //could be a copy constructor instead

    const c74::min::dict &toDict(); // TODO: MAKE PRIVATE

    double getRateOfImprovement() { return rateOfImprovement; }
    void setMutationRate(double mr) { mutationRate = mr; }

    void setMutationIndex(double mi) { mutationIndex = mi; }
    double getMutationRate() { return mutationRate; }
    void setAccuracy(double a);
    void setExpFactor(double ef);
    void setMaxPopulation(int mp);
    int getMaxPopulation() { return maxPopulation; }

    double getMaxFitness() { return maxFitness; }
    void calcFitness();

    // Compute average fitness for the population
    double getAverageFitness();

    // Compute the current "most fit" member of the population
    std::vector<int> &getBest(int &index);

    // Create a new generation
    void generate(double eta);

    void exponentialRanker(double c);

    DNA &rSelect();
    // https://stackoverflow.com/questions/24609131/implementation-of-roulette-wheel-selection
    DNA &select(double sum);

    bool terminate() { return finished; }

    int getNumberOfParams();
    
    std::vector<double> &getTargetParams(){
        return targetParams;
    }

    std::vector<int> getSelectionCount();

    int getGenerations() { return generations; }


};
