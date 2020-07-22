///// @file
/////    @ingroup     minexamples
/////    @copyright    Copyright 2018 The Min-DevKit Authors. All rights reserved.
/////    @license    Use of this source code is governed by the MIT License found in the License.md file.
//
#include "c74_min_unittest.h"    // required unit test header
#include "min.genetic.cpp"      // need the source of our object so that we can access it
//
//// Unit tests are written using the Catch framework as described at
//// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md
//
SCENARIO("object has the right number of params") {
    ext_main(nullptr);    // every unit test must call ext_main() once to configure the class

    GIVEN("An instance of our object whose population has been built with an arg of length 1") {

        test_wrapper<genetic> an_instance;
        genetic&    my_object = an_instance;
        const atoms& args= {1};
        my_object.buildPopulation(args);
        
        REQUIRE(my_object.getPopulation()->getNumberOfParams() == 1);


      }

}

TEST_CASE("object initialization"){
    ext_main(nullptr);
    test_wrapper<genetic> an_instance;
    genetic&    my_object = an_instance;
    const atoms& args= {3};
    my_object.buildPopulation(args);
    SECTION("build population with size 3"){
       
        REQUIRE(my_object.getPopulation()->getNumberOfParams() == 3);
        
    }
    SECTION("build population with size 3"){
         const atoms& args= {8};
         my_object.buildPopulation(args);
         REQUIRE(my_object.getPopulation()->getNumberOfParams() == 8);
         
     }
    SECTION("build population with size 3"){
            const atoms& args= {12};
            my_object.buildPopulation(args);
            REQUIRE(my_object.getPopulation()->getNumberOfParams() == 12);
            
        }
    
}

TEST_CASE("object initialization:calling buildPopulation sets default values for finished, mutationrate, maxPopulation and expFactor "){
    ext_main(nullptr);
    test_wrapper<genetic> an_instance;
    genetic&    my_object = an_instance;
    const atoms& args= {3};
    my_object.buildPopulation(args);
    SECTION("buildpopulation sets defaults"){

        REQUIRE(my_object.getPopulation()->finished== false);
        REQUIRE(my_object.getPopulation()->mutationRate == 0.001);
        REQUIRE(my_object.getPopulation()->maxPopulation== 11);
        REQUIRE(my_object.getPopulation()->expFactor== 0.975);
        
    }
    
    SECTION("population starts with average fitness of 0"){

        double expected = 0.0;
        REQUIRE(my_object.getPopulation()->getAverageFitness() < 0.0001);
    
    }
}

TEST_CASE("object's average fitness constantly improves"){
    ext_main(nullptr);
    test_wrapper<genetic> an_instance;
    genetic&    my_object = an_instance;
    const atoms& args= {3};
    
    my_object.buildPopulation(args);
    my_object.getPopulation()->setMaxPopulation(200);
    my_object.getPopulation()->targetParams = {1.0, 2.0, 3.0};
  
    SECTION("object can reach target"){
        
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
        REQUIRE(r== my_object.getPopulation()->targetParams);
       // std::cout << "generations on 1: " << my_object.getPopulation()->generations <<endl;
           
       }
    SECTION("object can reach target faster with settings:"){
           
          // my_object.getPopulation()->setMutationRate(0.5);
         
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
           //REQUIRE(r== my_object.getPopulation()->targetParams);
           //std::cout << "generations on 2: " << my_object.getPopulation()->generations <<endl;
          }
    
}
