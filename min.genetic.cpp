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
    outlet<> output {this, "(list) the dictionary of random values to be evaluated", "dictionary"};
    outlet<> output2 {this, "(list) frequency of values to examine mutation"};
    outlet<> output3{this, "(float) current max fitness"};
    
    void initializeObject(const atoms& args= {}){
        
        
        int t = (int) args[0];
        
       
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
        c74::max::t_atomarray* aa = population->toAtomArray();
        
        dictionary_appendlong(population->maxDict, c74::max::gensym("generation"), population->generations);
        dictionary_appendatomarray(population->maxDict, c74::max::gensym("population"), (c74::max::t_object*) aa);
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
    
    attribute<double> buildPopulation {this, "buildPopulation", {},
        setter { MIN_FUNCTION {
            
           if(population.get()){
                
                //population->targetParams.clear();
                //population->generations= 0;
                
                initializeObject(args);
                alreadyPrinted= false;
            }
            else if(args.size()>0){
                
                initializeObject(args);
                cout << "Object initialized" <<c74::min::endl;
                }
                population->popDict.touch();
                output.send("dictionary", population->popDict.name());
        return args;
    }}};
    
//    attribute<vector<double>> target {this, "target", {},
//        setter { MIN_FUNCTION {
//            
//           if(population.get()){
//                population->targetParams.clear();
//                population->generations= 0;
//                
//                initializeObject(args);
//                alreadyPrinted= false;
//            }
//            else if(args.size()>0){
//                
//                initializeObject(args);
//                cout << "Object initialized" <<c74::min::endl;
//                }
//                
//        return args;
//    }}};
                
  attribute<double> mutationRate {this, "mutationRate", 0.001,
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
            else return {0};
    }}};
                
                
   attribute<double>  expFactor {this, "expFactor", 0.975,
       setter { MIN_FUNCTION {
               
       if(population.get()){
               
           population->expFactor = double(args[0]); //TODO: make a setter!
               return {args[0]};
           }
               
           return {0};
               
           }},
          getter { MIN_GETTER_FUNCTION {
           if(population.get()){
         
             return {population->expFactor};
               
           }
           else return {0};
   }}};
                
   attribute<int> maxPopulation {this, "maxPopulation", 200,
            setter { MIN_FUNCTION {
                
                //cout << "args[0] " << int(args[0]) << c74::min::endl;
            if(population.get()){
            population->setMaxPopulation(int(args[0]));
            }
            return {args};
    }}};
           
  attribute<double> mutationIndex {this, "mutationIndex", 20.,
          setter { MIN_FUNCTION {
                              
       if(population.get()){
            population->setMutationIndex(int(args[0]));
      }
     
     return {args};
   }}};
   
    attribute<int> mutate{
    
                this, "mutate", 200 , setter{ MIN_FUNCTION {
                
                atom value= (atom)DNA::polynomialMutate(args[0], args[1]);
            
                output2.send(value);
                return {args};
    }}};
                
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
                
                
