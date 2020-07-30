///    @file
///    @ingroup     minexamples
///    @copyright    Copyright 2018 The Min-DevKit Authors. All rights reserved.
///    @license    Use of this source code is governed by the MIT License found in the License.md file.
// code based on Dan Schiffman's GA Tutorial from NOC: https://natureofcode.com/book/chapter-9-the-evolution-of-code/

#include "c74_min.h"
#include "population.h"

using namespace c74::min;




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
    outlet<> output {this, "(list) result of evolution"};
    outlet<> output2 {this, "(list) frequency of values to examine mutation"};
    outlet<> output3{this, "(float) current max fitness"};
    
    void initializeObject(const atoms& args= {}){
        cout <<"args size: " << args.size();
        
        vector<double> t;
        
        for(int i = 0; i< args.size(); i++){
            
            t.push_back((double) args[i]);
            
        }
        double oldMutationRate;
        double oldMaxPopulation;
        double oldExpFactor;
        bool reInit= false;
        if(population.get()){
            //save mutation rate
            oldMutationRate = population->mutationRate;
            oldMaxPopulation = population->maxPopulation;
            oldExpFactor = population->expFactor;
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
    
    
    
    attribute<vector<double>> target {this, "target", {},
        setter { MIN_FUNCTION {
            
           if(population.get()){
                population->targetParams.clear();
                population->generations= 0;
                
                initializeObject(args);
                alreadyPrinted= false;
            }
            else if(args.size()>0){
                
                initializeObject(args);
                cout << "Object initialized" <<c74::min::endl;
                }
                
        return args;
    }}};
                
                
  attribute<int> maxPopulation {this, "maxPopulation", 50,
              setter { MIN_FUNCTION {
                  
              
              if(population.get()){
              population->setMaxPopulation(int(args[0]));
              }
              return {args};
      }}};
             
    
  attribute<double> mutationRate {this, "mutationRate", 0.214,
          setter { MIN_FUNCTION {
                  
          if(population.get()){
                  
              population->setMutationRate(double(args[0]));
                
              }
                  
              return {args};
                  
      }}};
                      
  attribute<double> mutationIndex {this, "mutationIndex", 5.,
         setter { MIN_FUNCTION {
                             
      if(population.get()){
           population->setMutationIndex(int(args[0]));
     }
    
    return {args};
  }}};

                      
  attribute<double>  expFactor {this, "expFactor", 0.123,
      setter { MIN_FUNCTION {
              
      if(population.get()){
              
          population->setExpFactor(double(args[0]));
              
          }
              
          return {args};
                   
          
  }}};
                
//    attribute<int> mutate{
//    
//                this, "mutate", 200 , setter{ MIN_FUNCTION {
//                
//                if(population.get()){
//                //atom value= (atom)DNA::polynomialMutate(args[0] , population->mutationIndex);
//                output2.send(value);
//                }
//                return {args};
//    }}};
                
   message<> getMaxFitness {
   this, "getMaxFitness", "display the max fitness score.", MIN_FUNCTION {
       if(population.get()){
           
        double currentMax= (population->getMaxFitness()/population->targetParams.size()) * 100.;
           
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
                
                
