///// @file
/////    @ingroup     minexamples
/////    @copyright    Copyright 2018 The Min-DevKit Authors. All rights
/// reserved.
/////    @license    Use of this source code is governed by the MIT License
/// found in the License.md file.
//
#include "c74_min_unittest.h" // required unit test header
#include "dna.h"
#include "population.cpp"

//
//// Unit tests are written using the Catch framework as described at
//// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md
//

SCENARIO( "a population object has a default size of 10 and can be resized", "[vector]" ) {
    GIVEN( "A vector of DNA objects" ) {
        
        vector<double> t(100);
        unique_ptr<Population> population { nullptr };
        population = std::make_unique<Population>(t);
        REQUIRE( population->populationMembers.size() == 10);
        
        WHEN( "the size is increased" ) {
            population->setMaxPopulation(400);
            
            THEN( "the size and capacity change" ) {
                REQUIRE(population->populationMembers.size() == 400);
                
            }
        }
        
    }
    
}

SCENARIO( "a population object contains a set of target values", "[vector]" ) {
    GIVEN( "A vector of DNA objects" ) {

        vector<double> t = {125., 22.,201. };
        unique_ptr<Population> population { nullptr };
        population = std::make_unique<Population>(t);
        REQUIRE( population->targetParams == t);

        WHEN( "population is iterated over" ) {
            population->generate(20);

            THEN( "the population members have a fitness value" ) {
                REQUIRE(population->getAverageFitness() > 0.);

            }
        }

    }

}
SCENARIO( "exponentialRankSelector sorts a population's array of DNA objects in descending order", "[vector]" ) {
    GIVEN( "A vector of DNA objects" ) {

        vector<double> t = {125., 22.,201. };
        unique_ptr<Population> population { nullptr };
        population = std::make_unique<Population>(t);
        bool checker = 0;
        REQUIRE( population->targetParams == t);

        WHEN( "population is generated" ) {
            population->generate(20);
            population->exponentialRanker(0.01);



            for (int i = 0; i< population->populationMembers.size()-1; i++){

                if(population->populationMembers[i].fitness < population->populationMembers[i+1].fitness)checker = 1;
            };
        }
        THEN( "the population members are sorted" ) {
            REQUIRE(checker== 0);


        }

    }
}
//SCENARIO( "number of generations stays below 500 with default exponentialSelector val 0.975", "[vector]" ) {
//    GIVEN( "A vector of DNA objects" ) {
//
//        vector<double> t = {1., 2., 3., 4.};
//        unique_ptr<Population> population { nullptr };
//        population = std::make_unique<Population>(t);
//
//        int index;
//        std::vector<int>& currentBest= population->getBest(&index);
//        population->exponentialRanker(0.975);
//        population->setMutationIndex(20);
//        for(int i = 0; i< 10; i++){
//        WHEN( "population is generated") {
//
//            while(!population->finished){
//            population->generate(20);
//
//            currentBest= population->getBest(&index);
//
//            }
//
//
//        }
//
//       REQUIRE(population->generations < 600);
//            std::cout << "generations: " << population->generations<<endl;
//        }
//    }
//
//}

//SCENARIO( "number of generations stays below 500", "[vector]" ) {
//    GIVEN( "A vector of DNA objects" ) {
//
//        vector<double> t = {1., 2., 3.};
//        unique_ptr<Population> population { nullptr };
//        population = std::make_unique<Population>(t);
//
//        population->setExpFactor(0.4);
//        int index;
//        std::vector<int>& currentBest= population->getBest(&index);
//        for(int i = 0; i< 10; i++){
//        WHEN( "population is generated") {
//
//            while(!population->finished){
//            population->generate(20);
//
//            int index;
//            currentBest= population->getBest(&index);
//            //population->exponentialRankSelector(0.975);
//            }
//
//
//        }
//
//            std::cout << "generations needed : " << population->generations;
//        //REQUIRE(population->generations < 500);
//
//                   //cout << "generations: " << population->generations<<endl;
//        }
//    }

//}
