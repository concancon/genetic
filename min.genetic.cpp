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
    
    // As is common practice, we provide private memebers at the
    // end of the class definition.
    // Initializing values here still serves a purpose because this will occur
    // prior to the constructor being called.
    //not necessaray
    std::unique_ptr<Population> population { nullptr };
    
    //Population population;
    atoms result;
    vector<double> sVec;
    bool alreadyPrinted {false};
    
public:
    
    MIN_DESCRIPTION {"apply genetic algorithm to n params"};
    MIN_TAGS {"time"};
    MIN_AUTHOR {"Cycling 74"};
    MIN_RELATED {""};
    
    inlet<>  input {this, "(toggle) on/off"};
    outlet<> output {this, "(dict) the dictionary of random values to be evaluated", "dictionary"};
    outlet<> output2{this, "(list) best after accuracy thresh is passed"};
    outlet<> output3 {this, "(DNA) Current best, result"};
    
    
    
    std::unique_ptr<Population>& getPopulation(){
        return population;
    }
    
    vector<double>& getResultAsVector(){
        vector<double>* doubleResult = new vector<double>; //TODO: free this or improve it
        if(result.size()){
            for(auto r: result){
                doubleResult->push_back((double) r );
                
            }
            
        }
        return *doubleResult;
    }
    void initializeObject(const atoms& args= {}){
        
        
        int t = (int) args[0];
        
       
        double oldMutationRate;
        double oldMaxPopulation;
        double oldExpFactor;
        double oldmutationIndex;
        double oldAccuracy;
        bool reInit= false;
        if(population.get()){
            //save mutation rate
            oldMutationRate = population->mutationRate;
            oldMaxPopulation = population->maxPopulation;
            oldExpFactor = population->expFactor;
            oldmutationIndex = population->mutationIndex;
            oldAccuracy = population->accuracy;
            reInit= true;
        }
        population = std::make_unique<Population>(t);
    
		//notify max that these
        if(reInit){
            
            atoms a;
            a.push_back(oldMaxPopulation);
            maxPopulation.set(a);
            a.clear();
            a.push_back(oldMutationRate);
            mutationRate.set(a);
            a.clear();
            a.push_back(oldExpFactor);
            expFactor.set(a);
            
        }
    }
    

   //message to assign a fitness value to each member of a population
    message<> dictionary { this, "dictionary",
           "Dictionary containing the generation and fitness values for a population",
           MIN_FUNCTION {
              
            if(population.get()){
                //look at population->population[i-n]
                result.clear();
                
                if(args.size() == 0){
                    
                    cout << "we are going to use a vector instead of a dict for testing purposes" <<endl;
                    
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
                               population->population[i].fitness = val;
                           }
                           else {
                               cout << "missing key " << keyname << endl;
                           }
                        }
                       vector<double> occurences= population->displayPopulation();
                       int index;
                       std::vector<int>& currentBest = population->getBest(index);
                       for (auto it : currentBest) {
                               result.push_back(it);
                           }
                        //output current best
                        output3.send(result);

                      if(!(this->population->finished)){
                       
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
    message<> buildPopulation {this, "buildPopulation", "build an initial population", MIN_FUNCTION {
            
	   if(population.get()){

			//population->targetParams.clear();
			//population->generations= 0;
            population->population.clear();
                               
			initializeObject(args);
			alreadyPrinted= false;
		}
		else if(args.size()>0){
			initializeObject(args);
			cout << "Object initialized" <<c74::min::endl;
		}
		else {
			cout << "Missing arguments (number of parameters)" <<c74::min::endl;
			return args;
		}

	
        output.send("dictionary", population->toDict().name());
        return args;
    }};
    

                
  attribute<double> mutationRate {this, "mutationRate", 0.214,
        setter { MIN_FUNCTION {
                
        if(population.get()){
                
            population->mutationRate= double(args[0]);
                return {args[0]};
            }
                
            return {0};
                
            }},
           getter { MIN_GETTER_FUNCTION {
            if(population.get()){
          
              return {population->mutationRate};
                
            }
            else return {0.214};
    }}};
    
    attribute<double> mutationIndex {this, "mutationIndex", 5.,
            setter { MIN_FUNCTION {
                                
         if(population.get()){
              population->setMutationIndex(int(args[0]));
        }
       
       return {args};
     }}};

    attribute<double> accuracy {this, "accuracy", 95.0,
          setter { MIN_FUNCTION {
                  
          if(population.get()){
                  
              population->accuracy= double(args[0]);
                  return {args[0]};
              }
                  
              return {0};
                  
              }},
             getter { MIN_GETTER_FUNCTION {
              if(population.get()){
            
                return {population->accuracy};
                  
              }
              else return {95.0};
      }}};
                
   attribute<double>  expFactor {this, "expFactor", 0.123,
       setter { MIN_FUNCTION {
               
       if(population.get()){
               
           population->expFactor = double(args[0]); //TODO: make a setter!
               
           }
               
           return {args};
                    
           }},
          getter { MIN_GETTER_FUNCTION {
           if(population.get()){

             return {population->expFactor};

           }
           else return {0.123};
   }}};
                
   attribute<int> maxPopulation {this, "maxPopulation", 50,
            setter { MIN_FUNCTION {
                
                //cout << "args[0] " << int(args[0]) << c74::min::endl;
            if(population.get()){
            population->setMaxPopulation(int(args[0]));
            }
            return {args};
    }}};
           

   
    //attribute to test polynomialMutate method
    attribute<int> mutate{
    
                this, "mutate", 200 , setter{ MIN_FUNCTION {
                
                //atom value= (atom)DNA::polynomialMutate(args[0], args[1]);
            
                //output2.send(value);
                return {args};
    }}};
                
   message<> getMaxFitness {
   this, "getMaxFitness", "display the max fitness score.", MIN_FUNCTION {
       if(population.get()){
           
        double currentMax= (population->getMaxFitness()/population->numParams) * 100.;
           
         cout<< currentMax << c74::min::endl;
          //output3.send((atom)currentMax); //TODO: DISPLAY CURRENT MAX
       }
      
           return {};
    }};
   
                
    message<> bang {
        this, "bang", "test the functionality of DNA class.", MIN_FUNCTION {

                
            if(population.get()){

            if(!(this->population->finished)){
                //cout <<c74::min::endl;
                result.clear();
                //Create next generation
                population->generate(population->mutationIndex);

				int index;
                std::vector<int>& currentBest = population->getBest(index);
                for (auto it : currentBest) {
					result.push_back(it);
                }

                vector<double> occurences= population->displayPopulation();


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
                
                
