/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.


#include "c74_min.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cstdlib>

using namespace std;
class Population{
public:
    vector<int> targetParams;
    Population(){
        
        this->targetParams= {1, 2, 3};
    }
  
    
    
};




class DNA{
    
public:
    //constructor for DNA class
    DNA(int numberOfGenes){
        this->numberOfGenes= numberOfGenes; // change this to target.size()
        int randomGene= 0;
        for(int i = 0; i< numberOfGenes; i++){
            randomGene= rand() % 127;
            genes.push_back(randomGene);
        }
        
    }
    
    void fitnessFunction(vector<int> target){
        //our target will contain 3 elems to represent:
        // elem 0: osc pitch
        // elem 1: filter cutoff
        // elem 2: vca loudness
        int score = 0;
        for(int i = 0; i< genes.size(); i++){
            if(genes[i] == target[i]){
                score++;
            }
            // exponential fitting of score to fitness function to accentuate difference between a slightly better
            //result and its inferior
            fitness = pow(2, score);
        }
        
    }
    
    DNA crossover(DNA partner){
        
        DNA child(this->numberOfGenes);
        int midpoint = rand() % this->numberOfGenes;
        for(int i = 0; i< this->numberOfGenes; i++){
            if(i > midpoint){
                child.genes[i] = this->genes[i];
                
            }
            else{
              child.genes[i]= partner.genes[i];
            }
            
        }
        return child;
    }
    
    void mutate(float mutationRate){
        for(int i= 0; i< this->numberOfGenes; i++){
            double r = ((double) rand() / (RAND_MAX));
            if(r < mutationRate){
                this->genes[i] = rand() % 127;
            }
        }
        
    }
    
    void displayGenes(){
        for(auto it: genes){
            cout<< it <<std::endl;
        }
    }

   
private:
    
    std::vector<int> genes;
    float fitness= 0;
    int numberOfGenes;
    
};

using namespace c74::min;


class genetic : public object<genetic> {
public:
    
  genetic(const atoms& args = {}) {
      cout << "constructor called!" << c74::min::endl;
      Population population;
    
  }

    
	MIN_DESCRIPTION {"apply genetic algorithm to n params"};
	MIN_TAGS {"time"};
	MIN_AUTHOR {"Cycling '74"};
	MIN_RELATED {"min.beat.pattern, link.beat, metro, tempo, drunk"};

   
    
	inlet<>  input {this, "(toggle) on/off"};
	outlet<> bang_out {this, "(bang) triggers at randomized interval"};
	outlet<> interval_out {this, "(float) the interval for the current bang"};


    attribute<vector<double>> target {this, "target", {1.0, 1.0 ,1.0},
		setter { MIN_FUNCTION {
			//vector<c74::min::atom> values = args;
            //add this to the population class
            population.targetParams.clear();
            for(auto it: args){
                population.targetParams.push_back((int)it);
            }
            return {args};
        }}};


    message<> bang {
    this, "bang", "test the functionality of DNA class.", MIN_FUNCTION {


        //test.displayGenes();
        cout << "target is :" << c74::min::endl;
        for(auto it: population.targetParams){

            cout << it << c74::min::endl;
        }

        return {};
    }};


    private:
        Population population;


};


MIN_EXTERNAL(genetic);


