///// @file
/////    @ingroup     minexamples
/////    @copyright    Copyright 2018 The Min-DevKit Authors. All rights
///reserved.
/////    @license    Use of this source code is governed by the MIT License
///found in the License.md file.
//
#include "c74_min_unittest.h" // required unit test header
#include "dna.h"
#include "population.cpp" // need the source of our object so that we can access it
#include "min.genetic.cpp"
//
//// Unit tests are written using the Catch framework as described at
//// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md
//

TEST_CASE("object initialization") {
    ext_main(nullptr);
    c74::min::test_wrapper<genetic> an_instance;
    genetic &my_object = an_instance;
    const atoms &args = {3};
    my_object.buildPopulation(args);
    SECTION("build population with size 3") {

        REQUIRE(my_object.getPopulation()->getNumberOfParams() == 3);
    }
    SECTION("build population with size 8") {
        const atoms &args = {8};
        my_object.buildPopulation(args);
        REQUIRE(my_object.getPopulation()->getNumberOfParams() == 8);
    }
    SECTION("build population with size 12") {
        const atoms &args = {12};
        my_object.buildPopulation(args);
        REQUIRE(my_object.getPopulation()->getNumberOfParams() == 12);
    }
}
TEST_CASE("Fitness function assigns higher fitness values to members of the "
          "population that are closer to target") {
    ext_main(nullptr);
    SECTION(
        "Fitness of a vector with 0s is 100 percent given 0 randomize flag") {
        DNA dna(3, 0); // now all the 'random genes' should be set to 0
        dna.fitnessFunction({0, 0, 0});
        REQUIRE(dna.fitness == 100.0);
    }
    SECTION("Fitness of a vector with user specified gene values is 100 "
            "percent when matched by target vector") {
        DNA dna({3., 3., 3.}); // now all the 'random genes' should be set to 0
        dna.fitnessFunction({3., 3., 3.});
        REQUIRE(dna.fitness == 100.0);
    }
    SECTION("Fitness of a vector with user specified gene values is 50 percent "
            "when half of it is matched by target vector") {
        DNA dna({3., 3., 3., 6.}); // now we specify the genes
        dna.fitnessFunction({3., 3., 3., 3.});
        REQUIRE(dna.fitness == APPROX(99.7058823529));
    }
}

TEST_CASE("crossover function combines genes from two dna instances") {
    ext_main(nullptr);
    SECTION("crossover assigns genes from either calling dna or partner dna "
            "passed") {
        DNA dna({3., 3., 3., 3.});
        const DNA partner({7., 7., 7., 7.});
        dna.crossover(partner);

        for (auto gene : dna.genes) {

            REQUIRE(((gene == 7.) || (gene == 3.)));
        }
    }

    SECTION("crossover gives the same likelihood that a value from this or "
            "from partner is assigned to new vector (uniform distribution)") {

        int countThis = 0;
        int countPartner = 0;

        for (int i = 0; i < 1000; i++) {

            DNA dna({3., 3., 3., 3.});
            const DNA partner({7., 7., 7., 7.});
            dna.crossover(partner);
            for (auto gene : dna.genes) {

                if (gene == 3.) {
                    countThis++;
                } else {
                    countPartner++;
                }
            }
        }

        double thisVectorFrequency = (double)countThis / (1000. * 4.);
        double partnerVectorFrequency = (double)countPartner / (1000. * 4.);
        REQUIRE(thisVectorFrequency ==
                APPROX(partnerVectorFrequency).margin(0.09));
    }
}

double standardDeviation(vector<vector<int>> &mutatedValues) {
    std::vector<double> squaredDifferences;
    int sum;
    double variance;
    sum = std::accumulate(
        mutatedValues.begin(),
        mutatedValues.end(), // iterators for the outer vector
        0,                   // initial value for summation - 0
        [](int init, const std::vector<int>
                         &intvec) { // binaryOp that sums a single vector<int>
            return std::accumulate(
                intvec.begin(), intvec.end(), // iterators for the inner vector
                init);                        // current sum
                                              // use the default binaryOp here
        });

    double mean = sum / (double)mutatedValues.size();

    // Then for each number: subtract the Mean and square the result (the
    // squared difference).
    vector<vector<int>>::iterator row;
    vector<int>::iterator col;
    for (row = mutatedValues.begin(); row != mutatedValues.end(); row++) {
        for (col = row->begin(); col != row->end(); col++) {

            double result = (double)*col - mean;
            result *= result;
            squaredDifferences.push_back(result);
        }
    }
    // Then work out the average of those squared differences

    variance = std::accumulate(squaredDifferences.begin(),
                               squaredDifferences.end(), 0.);
    return sqrt(variance);
}

TEST_CASE("Polynomial mutation introduces perturbations in genes vector") {

    ext_main(nullptr);
    const int numberOfValues = 1000;
    double deviationForEtaOfFive = 0.;
    double deviationForEtaOfTwenty = 0.;
    double deviationForEtaOfOneHundred = 0.;

    SECTION("Perturbation occurs only within range specified by eta sub m") {
        bool geneInRange = true;
        DNA dna({3., 3., 3., 3.});
        dna.mutate(0.999, 25);

        for (int i = 0; i < 100; i++) {
            dna.mutate(0.999, 25);
            for (auto g : dna.genes) {
                if (!(g >= 0) && (g < 255))
                    geneInRange = false;
            }
        }

        REQUIRE(geneInRange == true);
    }
    SECTION("A eta sub m of 5 has a higher standard deviation than eta sub m "
            "of 20") {

        std::vector<vector<int>> mutatedValues;

        for (int i = 0; i < numberOfValues; i++) {
            DNA dna({100.});
            dna.mutate(1., 5.);
            mutatedValues.push_back(dna.genes);
        }

        deviationForEtaOfFive = standardDeviation(mutatedValues);
        mutatedValues.clear();
        for (int i = 0; i < numberOfValues; i++) {
            DNA dna({100.});
            dna.mutate(1., 20.);
            mutatedValues.push_back(dna.genes);
        }

        deviationForEtaOfTwenty = standardDeviation(mutatedValues);

        REQUIRE(deviationForEtaOfFive > deviationForEtaOfTwenty);
    }

    SECTION("An eta sub m of 20 has a higher standard deviation than an eta "
            "sub m of 100") {

        std::vector<vector<int>> mutatedValues;
        for (int i = 0; i < numberOfValues; i++) {
            DNA dna({100.});
            dna.mutate(1., 20.);
            mutatedValues.push_back(dna.genes);
        }

        deviationForEtaOfTwenty = standardDeviation(mutatedValues);

        mutatedValues.clear();

        for (int i = 0; i < numberOfValues; i++) {
            DNA dna({100.});
            dna.mutate(1., 100.);
            mutatedValues.push_back(dna.genes);
        }
        deviationForEtaOfOneHundred = standardDeviation(mutatedValues);
        REQUIRE(deviationForEtaOfTwenty > deviationForEtaOfOneHundred);
    }
}

TEST_CASE("Roulette Selection method works according to chromosome's fitness "
          "values") {
    ext_main(nullptr);
    test_wrapper<genetic> an_instance;
    genetic &my_object = an_instance;
    const atoms &args{10};
    my_object.buildPopulation(2);

    SECTION("a chromosome with a fitness of 30 will be selected about 30 "
            "percent of the time") {
        DNA dnaThirty({30.});
        DNA dnaSeventy({70.});
        int thirtyCounter = 0;
        int seventyCounter = 0;

        my_object.getPopulation()->populationMembers[0] = (dnaThirty);
        my_object.getPopulation()->populationMembers[1] = (dnaSeventy);

        my_object.getPopulation()->probabilityArray = {0.3, 0.7};

        for (int i = 0; i < 1000; i++) {
            DNA selected = my_object.getPopulation()->rSelect();

            for (auto g : selected.genes) {

                if (g == 30) {
                    ++thirtyCounter;
                } else if (g == 70) {
                    ++seventyCounter;
                } else {
                    std::cout << "something else : " << g << std::endl;
                }
            }
        }

        double seventyFrequency = seventyCounter / 1000.;
        double thirtyFrequency = thirtyCounter / 1000.;

        REQUIRE(((seventyFrequency == APPROX(0.7).margin(0.05) &&
                  thirtyFrequency == APPROX(0.3).margin(0.05))));
    }
}


TEST_CASE("exponentialSelector") {

    ext_main(nullptr);
    test_wrapper<genetic> an_instance;
    genetic &my_object = an_instance;
    const atoms &args = {3};

    my_object.buildPopulation(args);
    my_object.getPopulation()->setMaxPopulation(20);
    my_object.getPopulation()->targetParams = {1.0, 1.0, 1.0};

    double sum = 0.;
    int index = 0;
    
    my_object.dictionary(); // assign fitness scores according to target
           std::sort(my_object.getPopulation()->populationMembers.begin(),
           my_object.getPopulation()->populationMembers.end(),
           [](const DNA &a, const DNA &b) -> bool {
               return a.fitness > b.fitness;
           });
           my_object.getPopulation()->exponentialRanker(
                      my_object.getPopulation()->getExpFactor());

    
    SECTION("Probability Array is built such that the sum of all probabilites is 1 "){
        
               for (int i = 0; i < my_object.getPopulation()->populationMembers.size(); i++) {
                   sum += my_object.getPopulation()->probabilityArray[i];
               }

               REQUIRE(sum == APPROX(1.0)); // in between step
        
    }
    
    
    
    SECTION(" If c is set to zero only the best phenotype will be given a selection probability of 1") {
        bool homogenous= true;
        
        my_object.dictionary(); // assign fitness scores according to target
        
        //first check that we have a non-homegenous population
        for(auto dna: my_object.getPopulation()->populationMembers){
            for(auto g: dna.genes ){
                if(g != 1.0){
                    homogenous= false;
                }
            }
        }
        
        REQUIRE(homogenous == false);
           
     
        // create probabiltiy array
        my_object.getPopulation()->setExpFactor(0.0);// expected only the best phenotype should be selected
        // my_object.bang(); instead of calling bang lets test all of its
        // constituent steps. roughly speaking bang just calls generate getBest
        // and outputs the result generate
        // TODO: TEST THIS WITH ELITISM TOO!!!!!
        my_object.getPopulation()->exponentialRanker(
            my_object.getPopulation()->getExpFactor());

     
        REQUIRE(my_object.getPopulation()->probabilityArray[0] == 1);
        

    }


    SECTION(" If c is set to almost one all the phenotypes will have an equal likelihood of being selected") {

        // create probabiltiy array
        my_object.getPopulation()->setExpFactor(0.999);
                    // expect all members to have roughly the same probability of
                   // being chosen
        // my_object.bang(); instead of calling bang lets test all of its
        // constituent steps roughly speaking bang just calls generate getBest
        // and outputs the result generate
        // TODO: TEST THIS WITH ELITISM TOO!!!!!
        my_object.getPopulation()->exponentialRanker(
            my_object.getPopulation()->getExpFactor());

        bool probabilitiesAreSame= true;
        // here we can test the values in probabilityArray
        for(int i = 0; i< my_object.getPopulation()->probabilityArray.size(); i++){
            
            if( my_object.getPopulation()->probabilityArray[i] != APPROX(0.05).margin(.02)){
                probabilitiesAreSame= false;
            }
        }
        REQUIRE(probabilitiesAreSame==true);
    }
}

TEST_CASE("object's average fitness improves with a higher mutation rate") {

    ext_main(nullptr);
    long generationsOne = 0;
    long generationsTwo = 0;
    test_wrapper<genetic> an_instance;
    genetic &my_object = an_instance;
    const atoms &args = {3};

    my_object.buildPopulation(args);
    my_object.getPopulation()->setMaxPopulation(200);
    my_object.getPopulation()->targetParams = {1.0, 2.0, 3.0};

    SECTION("object can reach target") {

        my_object.accuracy = 100.0;

        while (my_object.getPopulation()->finished == false) {
            my_object.bang();
            my_object.getPopulation()->calcFitness();
        }

        vector<double> r = my_object.getResultAsVector();
        REQUIRE(r == my_object.getPopulation()->targetParams);
        generationsOne = my_object.getPopulation()->generations;
        delete my_object.doubleResult;
    }
    SECTION("object can reach target faster with mutationRate= 0.5") {

        my_object.getPopulation()->setMutationRate(0.5);
        my_object.accuracy = 100.0;
        my_object.dictionary(); // assign fitness scores according to target
        my_object.bang();

        while (my_object.getPopulation()->finished == false) {
            my_object.bang();
            my_object.getPopulation()->calcFitness();
            my_object.getPopulation()->getMaxFitness();
        }

        my_object.getPopulation()->getAverageFitness();
        vector<double> r = my_object.getResultAsVector();

        generationsOne = my_object.getPopulation()->generations;
        REQUIRE(r == my_object.getPopulation()->targetParams);
        REQUIRE(generationsTwo < generationsOne);
        delete my_object.doubleResult;
    }
}



