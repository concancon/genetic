#pragma once

#include "c74_min_api.h"
#include "dna.h"
#include "workerthread.h"
#include <iostream>
#include <random>
#include <thread>
#define USE_THREADS 0

#define DEFAULT_ACCURACY	(98.)
#define DEFAULT_MUTRATE		(0.214)
#define DEFAULT_MUTIDX		(5)
#define DEFAULT_MAXPOP		(10)
#define DEFAULT_EXPFACT		(0.123)

class Attributes {

public:

	Attributes()
	: accuracy {DEFAULT_ACCURACY}
	, expFactor {DEFAULT_EXPFACT}
	, mutationIndex {DEFAULT_MUTIDX}
	, mutationRate {DEFAULT_MUTRATE}
	, maxPopulation {DEFAULT_MAXPOP}
	{}

	virtual ~Attributes() = default;

	Attributes(const Attributes& a) = default;
	Attributes& operator=(const Attributes& a) = default;

	virtual void setAccuracy(double a) { accuracy = a; }
	virtual double getAccuracy() { return accuracy; }

	virtual void setExpFactor(double ef) { expFactor = ef; }
	virtual double getExpFactor() { return expFactor; }

	virtual void setMutationIndex(double mi) { mutationIndex = mi; }
	virtual double getMutationIndex() { return mutationIndex; }

	virtual void setMutationRate(double mr) { mutationRate = mr; }
	virtual double getMutationRate() { return mutationRate; }

	virtual void setMaxPopulation(int mp) { maxPopulation = mp; };
	virtual int getMaxPopulation() { return maxPopulation; }

private:

	double accuracy;
	double expFactor;
	double mutationIndex;
	double mutationRate;
	int maxPopulation;
};

class Population : public Attributes {
  public:
#if USE_THREADS
    const int numThreads = 4;
    std::unique_ptr<WorkerThread> workers[4];
#endif

	~Population() override = default;

	c74::min::dict popDict;
    c74::max::t_object *maxDict; // we need a t_object to write to dictionary
    std::vector<DNA> population;
    std::vector<double> targetParams;
    std::vector<double> counter;
    std::vector<double> probabilityArray;
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

	void setMaxPopulation(int mp) override; // note that this overrides the Attributes base class

	const c74::min::dict &toDict(); // TODO: MAKE PRIVATE

    double getRateOfImprovement() { return rateOfImprovement; }

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

    std::vector<double> &displayPopulation();
};
