///// @file
/////    @ingroup     minexamples
/////    @copyright    Copyright 2018 The Min-DevKit Authors. All rights reserved.
/////    @license    Use of this source code is governed by the MIT License found in the License.md file.
//
#include "c74_min_unittest.h"    // required unit test header
#include "min.genetic.cpp"      // need the source of our object so that we can access it
#include "dna.h"
//
//// Unit tests are written using the Catch framework as described at
//// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md
//



TEST_CASE("object initialization"){
    ext_main(nullptr);
    test_wrapper<genetic> an_instance;
    genetic&    my_object = an_instance;
    const atoms& args= {3};
    my_object.buildPopulation(args);
    SECTION("build population with size 3"){
       
        REQUIRE(my_object.getPopulation()->getNumberOfParams() == 3);
        
    }
    SECTION("build population with size 8"){
         const atoms& args= {8};
         my_object.buildPopulation(args);
         REQUIRE(my_object.getPopulation()->getNumberOfParams() == 8);
         
     }
    SECTION("build population with size 12"){
            const atoms& args= {12};
            my_object.buildPopulation(args);
            REQUIRE(my_object.getPopulation()->getNumberOfParams() == 12);
            
        }
    
}
TEST_CASE("Fitness function assigns higher fitness values to members of the population that are closer to target"){
    ext_main(nullptr);
    SECTION("Fitness of a vector with 0s is 100 percent given 0 randomize flag"){
        DNA dna(3, 0); // now all the 'random genes' should be set to 0
        dna.fitnessFunction({0, 0, 0});
        REQUIRE(dna.fitness == 100.0);
    }
    SECTION("Fitness of a vector with user specified gene values is 100 percent when matched by target vector"){
        DNA dna({3., 3., 3.}); // now all the 'random genes' should be set to 0
        dna.fitnessFunction({3., 3., 3.});
        REQUIRE(dna.fitness == 100.0);
    }
    SECTION("Fitness of a vector with user specified gene values is 50 percent when half of it is matched by target vector"){
           DNA dna({3., 3., 3., 6.}); // now we specify the genes 
           dna.fitnessFunction({3., 3., 3., 3.});
           REQUIRE(dna.fitness == APPROX(99.7058823529));
       }
    
}

TEST_CASE("crossover function combines genes from two dna instances"){
    ext_main(nullptr);
    SECTION("crossover assigns genes from either calling dna or partner dna passed"){
        DNA dna({3., 3., 3., 3.});
        const DNA partner({7., 7., 7., 7.});
        dna.crossover(partner);
        
        for(auto gene: dna.genes){
            
             REQUIRE (((gene == 7.) || (gene == 3.)));
            
        }
       }
    
     SECTION("crossover gives the same likelihood that a value from this or from partner is assigned to new vector (uniform distribution)"){
         
         int countThis = 0;
         int countPartner = 0;
         
         
         for(int i = 0; i < 1000; i++){
                        
            DNA dna({3., 3., 3., 3.});
            const DNA partner({7., 7., 7., 7.});
             dna.crossover(partner);
             for(auto gene: dna.genes){
            
                 if(gene== 3.){
                     countThis++;
                 }
                 else{
                     countPartner++;
                 }
             }
         }

         double thisVectorFrequency = (double) countThis / (1000.* 4.) ;
         double partnerVectorFrequency = (double)countPartner /(1000.* 4.);
         REQUIRE (thisVectorFrequency == APPROX(partnerVectorFrequency).margin(0.09));
         
         
     }
    

}

TEST_CASE("Polynomial mutation introduces perturbations in genes vector"){
    ext_main(nullptr);
    SECTION("Perturbation occurs only within range specified by eta sub m"){
        bool geneInRange = true;
        DNA dna({3., 3., 3., 3.});
        dna.mutate(0.999, 25);
        
        for(int i = 0; i < 100; i++){
            dna.mutate(0.999, 25);
            for(auto g : dna.genes){
                if(!(g >=0) && (g< 255))geneInRange = false;
                
                }
            }
            REQUIRE(geneInRange == true);
    
    }
    SECTION("An eta sub m value of 20 has a big standard deviation"){
        DNA dna({3.});
        std::vector<vector<int>> mutatedValues;
        
        for(int i = 0; i< 1000; i++){
            
            dna.mutate(1., 100.);
            for(auto g: dna.genes){
                
                std::cout<< g<<std::endl;
            }
            mutatedValues.push_back(dna.genes);
        
        }
       
    }
}


TEST_CASE("object's average fitness improves with a higher mutation rate"){
    
    long generationsOne= 0;
    long generationsTwo= 0;
    ext_main(nullptr);
    test_wrapper<genetic> an_instance;
    genetic&    my_object = an_instance;
    const atoms& args= {3};
    
    my_object.buildPopulation(args);
    my_object.getPopulation()->setMaxPopulation(200);
    my_object.getPopulation()->targetParams = {1.0, 2.0, 3.0};
    
  
    SECTION("object can reach target"){
        
        my_object.accuracy = 100.0;

      
        while(my_object.getPopulation()->finished == false){
           my_object.bang();
           my_object.getPopulation()->calcFitness();
           
        }
        
        vector<double> r= my_object.getResultAsVector();
        REQUIRE(r== my_object.getPopulation()->targetParams);
        generationsOne= my_object.getPopulation()->generations;
        delete my_object.doubleResult;
       }
    SECTION("object can reach target faster with mutationRate= 0.5"){
           
           my_object.getPopulation()->setMutationRate(0.5);
           my_object.accuracy = 100.0;
           my_object.dictionary(); //assign fitness scores according to target
           my_object.bang();
           
           while(my_object.getPopulation()->finished == false){
              my_object.bang();
              my_object.getPopulation()->calcFitness();
              my_object.getPopulation()->getMaxFitness();
           }
           
           my_object.getPopulation()->getAverageFitness() ;
           vector<double> r= my_object.getResultAsVector();
         
           generationsOne= my_object.getPopulation()->generations;
           REQUIRE(r== my_object.getPopulation()->targetParams);
           REQUIRE(generationsTwo < generationsOne);
           delete my_object.doubleResult;
          }
    
}

TEST_CASE("exponentialSelector"){
    
    ext_main(nullptr);
    test_wrapper<genetic> an_instance;
    genetic&    my_object = an_instance;
    const atoms& args= {3};
    
    my_object.buildPopulation(args);
    my_object.getPopulation()->setMaxPopulation(20);
    my_object.getPopulation()->targetParams = {1.0, 2.0, 3.0};
    
    double sum = 0;
    int index = 0;
    SECTION(" If c is set to zero only the best phenotype will be selected"){
       
      
        my_object.dictionary(); //assign fitness scores according to target
       //sort according to fintess
        std::sort(my_object.getPopulation()->population.begin(), my_object.getPopulation()->population.end(), [](const DNA& a, const DNA& b) -> bool { return a.fitness > b.fitness; });
          //create probabiltiy array
        my_object.getPopulation()->expFactor = 0.0; //expected only the best phenotype should be selected
        //my_object.bang(); instead of calling bang lets test all of its constituent steps
        //roughly speaking bang just calls generate getBest and outputs the result
        //generate
        //TODO: TEST THIS WITH ELITISM TOO!!!!!
        my_object.getPopulation()->exponentialRankSelector(my_object.getPopulation()->expFactor);

        for (int i = 0; i<my_object.getPopulation()->population.size(); i++){
               sum+= my_object.getPopulation()->probabilityArray[i];
           }
    
        REQUIRE(sum== 1); //in between step
        DNA partnerA = my_object.getPopulation()->rSelect();
        REQUIRE(partnerA.genes.size()==3);
        REQUIRE(partnerA.genes == my_object.getPopulation()->getBest(index));
        
    }
    
}

TEST_CASE("exponentialSelector 2"){
    
    ext_main(nullptr);
    test_wrapper<genetic> an_instance;
    genetic&    my_object = an_instance;
    const atoms& args= {3};
    
    my_object.buildPopulation(args);
    my_object.getPopulation()->setMaxPopulation(20);
    my_object.getPopulation()->targetParams = {1.0, 2.0, 3.0};
    
    double sum = 0.;
    int index = 0;
    SECTION(" If c is set to almost one all the phenotypes will be selected"){
       
      
        my_object.dictionary(); //assign fitness scores according to target
       //sort according to fintess
        std::sort(my_object.getPopulation()->population.begin(), my_object.getPopulation()->population.end(), [](const DNA& a, const DNA& b) -> bool { return a.fitness > b.fitness; });
          //create probabiltiy array
        my_object.getPopulation()->expFactor = 0.999; //expect all members to have roughly the same probability of being chosen
        //my_object.bang(); instead of calling bang lets test all of its constituent steps
        //roughly speaking bang just calls generate getBest and outputs the result
        //generate
        //TODO: TEST THIS WITH ELITISM TOO!!!!!
        my_object.getPopulation()->exponentialRankSelector(my_object.getPopulation()->expFactor);

        //here we can test the values in probabilityArray
        
        for (int i = 0; i<my_object.getPopulation()->population.size(); i++){
               sum+= my_object.getPopulation()->probabilityArray[i];
           }

        REQUIRE( sum == Approx(1.) );
        
        for(int i = 0; i < 200;i++ ){
            my_object.getPopulation()->rSelect();
        }
        vector<int> ans = my_object.getPopulation()->getSelectionCount();
      
        
        
        //std::cout << ans<<endl;
  
        
    }
    
}
