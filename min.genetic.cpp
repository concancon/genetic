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
    //Population population;
    vector<int> currentBest;
    atoms result;
    vector<int> sVec;
    int populationSize= 0;
    // initialized first!
    // CRITICAL because other member initialization below relies on this value!
    bool initialized { false };
    
public:
    
    MIN_DESCRIPTION {"apply genetic algorithm to n params"};
    MIN_TAGS {"time"};
    MIN_AUTHOR {"Cycling 74"};
    MIN_RELATED {""};

   
  
    inlet<>  input {this, "(toggle) on/off"};
    outlet<> output {this, "(list) result of evolution"};
    
    // constructor will be called AFTER *all* of the members
    // (most of which are located below in this class definition).
    genetic(const atoms& args = {}) {
      
        // processing our arguments makes an assignment to the
        // tempo attribute --
        // okay because it will have already been initialized with the members
        if (args.size() > 0)
         populationSize = args[0];
        
          // create an instance of "beat_generator" which is a class defined
        // in a library or in another piece of code we authored elsewhere.
        population = std::make_unique<Population>(sVec);
      
          // now that m_beat_generator is valid and our object is properly
        // initialized, we can switch our flag used to prevent unsafe access
        // in the attribute setters (below)
        initialized = true;
    }
    
   
    attribute<vector<double>> target {this, "target", {1, 1 ,1, 1},
        setter { MIN_FUNCTION {
          
            for(auto it: args){
                       sVec.push_back((int)it);
                           }
            
        
            return {args};
        }}};
    
    attribute<double> mutationRate {this, "mutationRate", 0.01,
        setter { MIN_FUNCTION {
            
            if(this->initialized){
            population->setMutationRate(double(args[0]));
            }
            return {args};
        }}};
            
            
    attribute<int> maxPopulation {this, "maxPopulation", 100,
            setter { MIN_FUNCTION {

            //cout << "args[0] " << int(args[0]) << c74::min::endl;
                if(this->initialized){
                population->setMaxPopulation(int(args[0]));
                }
                return {args};
            }}};


    message<> bang {
    this, "bang", "test the functionality of DNA class.", MIN_FUNCTION {
    
        if(this->initialized){
        //cout <<c74::min::endl;
        currentBest.clear();
        result.clear();
       
        if(!population->terminate()){
        
        //Create next generation
        population->generate();
        // Calculate fitness
        population->calcFitness();

        currentBest= population->getBest();
       
        }
        for(auto it : currentBest){
            result.push_back(it);
            
        }
  
        //cast current best to atoms
        
        output.send(result);
        }
        return {};
    }};
        
private:
    // As is common practice, we provide private memebers at the
    // end of the class definition.
    // Initializing values here still serves a purpose because this will occur
    // prior to the constructor being called.
    std::unique_ptr<Population> population { nullptr };
    


};


MIN_EXTERNAL(genetic);
