///    @file
///    @ingroup     minexamples
///    @copyright    Copyright 2018 The Min-DevKit Authors. All rights reserved.
///    @license    Use of this source code is governed by the MIT License found in the License.md file.
// code based on Dan Schiffman's GA Tutorial from NOC: https://natureofcode.com/book/chapter-9-the-evolution-of-code/

#include "c74_min.h"
#include "population.h"

using namespace c74::min;
using namespace c74::max;

class genetic : public object<genetic> {
    
private:
    //population object to represent a collection of dna's, their fitness values and the genetic operators
    std::unique_ptr<Population> population { nullptr };
    //Dummy object used for caching values
    Population popDummy{0};
    //used to represent genes as an atom type. Max needs this.
    atoms result;
    //flag to determine whether or not we have already printed the final result of our algorithm
    bool alreadyPrinted {false};
    
    
    
public:
    
    vector<double>* doubleResult;
    MIN_DESCRIPTION {"apply genetic algorithm to n params"};
    MIN_TAGS {"time"};
    MIN_AUTHOR {"Cycling 74"};
    MIN_RELATED {""};
    
    inlet<>  input {this, "(toggle) on/off"};
    outlet<> output {this, "(dict) the dictionary of random values to be evaluated", "dictionary"};
    outlet<> output2{this, "(list) best after accuracy thresh is passed"};
    outlet<> output3 {this, "(DNA) Current best, result"};
    
    
    vector<double>& getResultAsVector(){
          
          if(result.size()){
              for(auto r: result){
                  doubleResult->push_back((double) r );
              }
          }
          return *doubleResult;
      }
    
    std::unique_ptr<Population>& getPopulation(){
        return population;
    }
    
    void initializeObject(const atoms& args= {}){
        
        int t = (int) args[0];
        if(population.get()){
         popDummy.adoptAttributes(*population);
         }
         
        population = std::make_unique<Population>(t);
        population->adoptAttributes(popDummy);
  
    }
 
    //message to assign a fitness value to each member of a population
    message<> dictionary { this, "dictionary",
           "Dictionary message assigns the incoming fitness values to the population inside this object",
           MIN_FUNCTION {
              
            if(population.get()){
                result.clear();
                
                if(args.size() == 0){
                   population->calcFitness();
                }
                else{
                    try {
                       dict d = {args[0]};
                       t_dictionary *popd;
                        if (dictionary_getdictionary(d, gensym("population"), &popd) == MAX_ERR_NONE) {
                           long size = dictionary_getentrycount(popd);
                            for (long i = 0; i < size; i++) {
                               char keyname[256];
                               double val;
                               snprintf(keyname, 256, "pop_%ld", i);
                               if (dictionary_getfloat(popd, gensym(keyname), &val) == MAX_ERR_NONE) {
                                   population->population[i].fitness = val; // HERE WE ASSIGN THE RECEIVED FITNESS VALUES TO THE POPULATION :)
                               }
                               else {
                                   cout << "missing key " << keyname << endl;
                               }
                            }
                           
                           int index;
                           std::vector<int>& currentBest = population->getBest(index);
                           if(std::find(currentBest.begin(), currentBest.end(), -1) == currentBest.end())
                            {
                               for (auto it : currentBest) {
                                       result.push_back(it);
                                   }
                               
                            }
                           output3.send(result);
                           if(!(population->finished)){
                           
                                population->generate(population->mutationIndex);
                                //create a dictionary once again with the new population
                                output.send("dictionary", population->toDict().name());
                            }
                            else{
                                cout << "we are finished!" << c74::min::endl;
                                output2.send(result);
                            }
                       }
                   }
                   catch (std::exception& e) {
                       cerr << e.what() << endl;
                            }
                    }
            }
            else{
                 cout << "initialize population before trying to pass in an array of fitness vals" <<c74::min::endl;
                }
               return {};
           }
       };
                    
    message<> buildPopulation {this, "buildPopulation", "build an initial population of n params", MIN_FUNCTION {
      
	   if(population.get()){
            population->population.clear();
			initializeObject(args);
			alreadyPrinted= false;
		}
		else if(args.size()>0){
			initializeObject(args);
	
		}
		else {
			cout << "Missing arguments (number of parameters)" <<c74::min::endl;
			return args;
		}
        output.send("dictionary", population->toDict().name());
        return args;
    }};
    

    attribute<double> accuracy {this, "accuracy", DEFAULT_ACCURACY,
          setter { MIN_FUNCTION {
                 
           Population &pop = population.get() ? *population : popDummy ;
           pop.setAccuracy(double(args[0]));
           return {args};
                  
      }}};
            
   attribute<int> maxPopulation {this, "maxPopulation", DEFAULT_MAXPOP,
          setter { MIN_FUNCTION {
         Population &pop = population.get() ? *population : popDummy ;
        
              pop.setMaxPopulation(int(args[0]));
            
            return {args};
    }}};
           
  
   attribute<double> mutationRate {this, "mutationRate", DEFAULT_MUTRATE,
        setter { MIN_FUNCTION {
                
           Population &pop = population.get() ? *population : popDummy ;
          pop.setMutationRate(double(args[0]));
           
                
        return {args};
                
    }}};
                    
   attribute<double> mutationIndex {this, "mutationIndex", DEFAULT_MUTIDX,
       setter { MIN_FUNCTION {
          Population &pop = population.get() ? *population : popDummy ;
          
           pop.setMutationIndex(int(args[0]));
           
  
        return {args};
    }}};

                    
   attribute<double>  expFactor {this, "expFactor", DEFAULT_EXPFACT,
     setter { MIN_FUNCTION {
            
      Population &pop = population.get() ? *population : popDummy ;
        pop.setExpFactor(double(args[0]));
        
            
        return {args};
    }}};
                    
    message<> getMaxFitness {this, "getMaxFitness", "display the max fitness score.", MIN_FUNCTION {
       
      if(population.get()){
           double currentMax= (population->getMaxFitness()/population->numParams) * 100.;
           cout<< currentMax << c74::min::endl;
       }
      
       return {};
    }};
   
    message<> bang {
        this, "bang", "test the functionality of DNA class.", MIN_FUNCTION {

                
            if(population.get()){

                if(!(population->finished)){
                   
                    result.clear();
                    //Create next generation
                    population->generate(population->mutationIndex);
                    int index;
                    std::vector<int>& currentBest = population->getBest(index);

                    for (auto it : currentBest) {
                        result.push_back(it);
                    }
                    output.send(result);
                }
                else if (!alreadyPrinted){

                    cout << "as close as youre going to get! " <<c74::min::endl;
                    cout << "generations: " << population->generations << c74::min::endl;
                    alreadyPrinted = true;
                            }
                }
                        
         return {};
      }};
      
};
                
                
        MIN_EXTERNAL(genetic);
                
                
