#include "population.h"
#include <math.h>
#include <iostream>
#include <random>
#include <numeric>
#include <chrono>
//#include "c74_min.h"

using namespace std;
using namespace std::chrono;


//using namespace c74::max;
struct LightIterator : public std::vector<DNA>::iterator
{
    LightIterator(std::vector<DNA>::iterator it) : std::vector<DNA>::iterator(it) {}
    double& operator*() { return std::vector<DNA>::iterator::operator*().fitness; }
};

//create a population with default values for mutation rate and population size.
//calculate the value of the perfect
//PARAMS: target params: input from max. For example a set of pixels
Population::Population(const vector<double>& tp):  counter(256) {
    
   // maxDict = (t_object*)popDict; //produces a memory leak //TODO: CREATE DESTRUCTOR TO RELEASE THIS
    expFactor = 0.204;
    targetParams= tp;
    finished = false;
    population.clear();
    mutationRate = 0.001;
    maxPopulation = 40;
    mutationIndex = 10;
    
    for(int i = 0; i< maxPopulation; i++) {
        DNA dna(targetParams.size(), true);
        population.push_back(std::move(dna));
    }

#if USE_THREADS
	for (int i = 0; i < numThreads; i++) {
		workers[i] = std::make_unique<WorkerThread>();
	}
#endif
    calcFitness();
}
   

//c74::max::t_atomarray* Population::toAtomArray(){
//    
//    long ac = maxPopulation;
//    t_atom* av = (t_atom*)sysmem_newptr(sizeof(t_atom)* ac);
//    long idx = 0;
//    
//    for(auto pop : population){
// 
//        atom_setobj(av+idx++, (t_object*)pop.toAtomArray());
//        
//        
//    }
//    
//    t_atomarray* aa = atomarray_new(ac, av);
//    atomarray_flags(aa, 1); // this takes care of freeing dna objects memory once the memory for population is freed
//    sysmem_freeptr(av);
//    return aa;
//    
//}
//setter for the population size
void Population::setMaxPopulation(int mp){
    maxPopulation = mp;
    population.clear();
    for (int i = 0; i < maxPopulation; i++) {
        DNA dna(targetParams.size(), true);
        population.push_back(std::move(dna));
    }
    calcFitness();
}

// uncomment to time the function, it's definitely faster
// #define BENCHMARK

//Iterate through the population to calculate the fitness of each individual therein
void Population::calcFitness(){

   
//    t_atomarray* aa = toAtomArray();
//    dictionary_appendlong(maxDict, gensym("generation"), generations);
//    dictionary_appendatomarray(maxDict, gensym("population"), (t_object*) aa);
//
#ifdef BENCHMARK
	auto start = high_resolution_clock::now();
#endif

#if USE_THREADS
	int div = population.size() / numThreads;

	for (int i = 0; i < numThreads; i++) {
		workers[i]->doAsync([this, div, i] {  // add new tasks
			auto it1 = population.begin() + (i * div);
			auto it2 = (i == numThreads - 1) ? population.end() : (it1 + div); //if we are in the last thread then the end has to be rounded to end of the population.
			for ( ; it1 < it2; it1++) { //two iterators to address sub arrays of the population
				it1->fitnessFunction(targetParams);
			}
		});
	}
	for (int i = 0; i < numThreads; i++) {
		workers[i]->wait();  // wait for all threads to be finished. for synchronisation. pattern is called barrier.
	}
#else
	for (auto it = population.begin(); it < population.end(); it++) {
		it->fitnessFunction(targetParams);
	}
#endif

#ifdef BENCHMARK
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	std::cout << "calcFitness: " << duration.count() << std::endl;
#endif

}

// Compute average fitness for the population
double Population::getAverageFitness() {
    double total = 0.0;
    total = std::accumulate(LightIterator{population.begin()}, LightIterator{population.end()}, (double)0.0);
        
    return (total / (double)population.size());
}
//get the fittest member in the population.
//we use this to output the best member to Max
vector<int>& Population::getBest(int& index) {

	static vector<int> defaultGenes;
    vector<int> diff;
    
	maxFitness = 0.;
	index= -1;
    for (int i = 0; i < population.size(); i++) {
        if (population[i].fitness > maxFitness) {
            maxFitness = population[i].fitness;
            index = i;
        }
    }
  
    if (maxFitness >= targetParams.size() * 0.99) {
        finished = true;
    }
	if (index >= 0) {
        
        diff.clear();
       
        //calculate the amount of elements that have changed since last call
        std::set_difference(population[index].genes.begin(),
                            population[index].genes.end(),
                            lastBest.begin(),
                            lastBest.end(),
                                 std::inserter(diff, diff.begin()));
        
         //if(lastBest.size()) lastBest.clear();
        double rateOfImprovement = (double) diff.size() /(double)population[index].genes.size();
        cout << rateOfImprovement << " rate of improvement" << endl;
        for (auto i : diff) std::cout << i << ' ';
        cout <<endl;
        //lastBest.swap(population[index].genes);
    
        lastBest = population[index].genes;
        return population[index].genes;
	}
	return defaultGenes;
}

//This method replaces what used to be the natural selection method.
//it is based on the idea of setting each fitness value to a percentage in
//relation to the sum of all individuals.
//This percentage is then used when selecting a score, whereby a member with a higher score has a greater
//probability of being chosen.
void Population::generate(double mutationIndex) {
    
    double sum = 0.;
    // Refill the population with children from the mating pool
    newPopulation.clear();
    //newPopulation.shrink_to_fit();
    vector<double> scores(population.size());
    
//    double sum = std::accumulate(LightIterator{population.begin()}, LightIterator{population.end()}, (double) 0.0);
//    double inverseSum = 1.0 / sum;
//    for (int i = 0; i < population.size(); i++) {
//        scores[i] = population[i].fitness * inverseSum;
//    }
    std::sort(population.begin(), population.end(), [](const DNA& a, const DNA& b) -> bool { return a.fitness > b.fitness; });
    int elitelen = population.size() * 0.1;
    for (int i = 0; i < elitelen; i++) {
        newPopulation.push_back(population[i]);
    }

    exponentialRankSelector(expFactor);
    for (int i = 0; i<population.size(); i++){
        sum+=probabilityArray[i];
    }
    
    for (int i = 0; i < population.size() - elitelen; i++) {
        DNA partnerA = select(sum);
        DNA partnerB = select(sum);
        DNA child = partnerA.crossover(partnerB); // this should be moved or elided, thus ok
        child.mutate(mutationRate, mutationIndex);
        newPopulation.push_back(std::move(child)); //std::move(child));
    }
    population.swap(newPopulation);
    //population= newPopulation;
    generations++;
    
    calcFitness();
}


std::vector<double>& Population::exponentialRankSelector(double c){
   
   probabilityArray.clear();
    //first we need to sort the array in descending order
   //std::sort(population.begin(), population.end(), [](const DNA& a, const DNA& b) -> bool { return a.fitness > b.fitness; });

    for(int i = 0; i< population.size(); i++){
       
        double numerator = c-1;
        double denominator=  pow(c, population.size()) -1.;
        double probability = pow(c, i) *  (numerator/ denominator);
        probabilityArray.push_back(probability);
    }
    return probabilityArray;
}
//select based on exponentialSelector
DNA& Population::select(double sum) {
    
    double random = (double)equalRandom(gen) * sum ;

    int index = 0;
    for ( ; random > 0. && index <probabilityArray.size(); index++) {
        random -= probabilityArray[index];
    }
    if (random > 0.0) {
        cout << "random is greater than 0!" << endl;
    }
    index--;

    population[index].count++;
    return population[index];
    
    
}

DNA& Population::rSelect(){
    double rndNumber = rand() / (double) RAND_MAX;
    double offset = 0.0;
    int pick = 0;

    for (int i = 0; i < population.size(); i++) {
        offset += probabilityArray[i];
        if (rndNumber < offset) {
            pick = i;
            break;
        }
        
    }
    cout << "chromosome selected: " ;
    population[pick].displayGenes();
    return population[pick];
   
    
}

//choose a single member of the population based on its score
DNA& Population::select(const vector<double>& scores, double sum) {
    double random = (double)equalRandom(gen) * sum;

    int index = 0;
    for ( ; random > 0. && index < scores.size(); index++) {
        random -= scores[index];
    }
    if (random > 0.0) {
        cout << "random is greater than 0!" << endl;
    }
    index--;

    population[index].count++;
    return population[index];
}
vector<double>& Population::displayPopulation() {
	for (int i = 0; i < population.size(); i++) {
        for (int j = 0; j < population[i].genes.size(); j++) {
            //count the occurence of each gene
            counter[population[i].genes[j]]++;
        }
    }
    return counter;
}

void Population::setExpFactor(double ef){
    
    expFactor= ef;
    
}
