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
    vector<double> sVec;
    bool initialized { false };
    bool alreadyPrinted {false};
    
public:
    
    MIN_DESCRIPTION {"apply genetic algorithm to n params"};
    MIN_TAGS {"time"};
    MIN_AUTHOR {"Cycling 74"};
    MIN_RELATED {""};

    inlet<>  input {this, "(toggle) on/off"};
    outlet<> output {this, "(list) result of evolution"};
    
    void initializeObject(const atoms& args= {}){
        cout <<"args size: " << args.size();
   
        vector<double> t;
   
        for(int i = 0; i< args.size(); i++){
            
            t.push_back((double) args[i]);
   
            }

        population = std::make_unique<Population>(t);
      
        this->initialized= true;
        
    }
    
    // constructor will be called AFTER *all* of the members
    // (most of which are located below in this class definition).
    genetic(const atoms& args= {}) {
      
      //NOTHING OF INTEREST HERE
            
        }
    
    
   
    attribute<vector<double>> target {this, "target", {},
        setter { MIN_FUNCTION {
            vector<double> params;
            if(this->initialized){
                population->targetParams.clear();
                population->generations= 0;
                for(auto it: args){
                    params.push_back((double)it);
                }

                initializeObject(args);
                alreadyPrinted= false;
            }
            else if(args.size()>0){
              
                initializeObject(args);
                cout << "Object initialized" <<c74::min::endl;
            }
        
            return args;
        }}};
    
    attribute<double> mutationRate {this, "mutationRate", 0.06,
        setter { MIN_FUNCTION {
            
            if(this->initialized){
            population->mutationRate= double(args[0]);
           
            }
           
            return {args};
           
        }}};
            
            
    attribute<int> maxPopulation {this, "maxPopulation", 1000,
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
       
        if(!(this->population->finished)){
        //cout <<c74::min::endl;
        currentBest.clear();
        result.clear();
        //Create next generation
        population->generate();
        // Calculate fitness
        population->calcFitness();

        currentBest= population->getBest();
       
        
        for(auto it : currentBest){
            result.push_back(it);
            
        }
  
        //cast current best to atoms
        
        output.send(result);
        }
        else if (!alreadyPrinted){
            
            cout << "already finished! " <<c74::min::endl;
            cout << "generations: " << population->generations << c74::min::endl;
            alreadyPrinted = true;
                }
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
