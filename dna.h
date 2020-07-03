#pragma once

#include <vector>
#include <random>
#include <iostream>
//=============================================================================*/

#include <stdbool.h>
#include <stdint.h>
#include "platform.h"
#include "internals.h"
#include "specialize.h"
#include "softfloat.h"

float64_t f128_to_f64( float128_t a )
{
    union ui128_f128 uA;
    uint_fast64_t uiA64, uiA0;
    bool sign;
    int_fast32_t exp;
    uint_fast64_t frac64, frac0;
    struct commonNaN commonNaN;
    uint_fast64_t uiZ;
    struct uint128 frac128;
    union ui64_f64 uZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uA.f = a;
    uiA64 = uA.ui.v64;
    uiA0  = uA.ui.v0;
    sign  = signF128UI64( uiA64 );
    exp   = expF128UI64( uiA64 );
    frac64 = fracF128UI64( uiA64 );
    frac0  = uiA0;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( exp == 0x7FFF ) {
        if ( frac64 | frac0 ) {
            softfloat_f128UIToCommonNaN( uiA64, uiA0, &commonNaN );
            uiZ = softfloat_commonNaNToF64UI( &commonNaN );
        } else {
            uiZ = packToF64UI( sign, 0x7FF, 0 );
        }
        goto uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    frac128 = softfloat_shortShiftLeft128( frac64, frac0, 14 );
    frac64 = frac128.v64 | (frac128.v0 != 0);
    if ( ! (exp | frac64) ) {
        uiZ = packToF64UI( sign, 0, 0 );
        goto uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    exp -= 0x3C01;
    if ( sizeof (int_fast16_t) < sizeof (int_fast32_t) ) {
        if ( exp < -0x1000 ) exp = -0x1000;
    }
    return
        softfloat_roundPackToF64(
            sign, exp, frac64 | UINT64_C( 0x4000000000000000 ) );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





static std::random_device rad;  //Will be used to obtain a seed for the random number engine
static std::mt19937 engine{rad()}; //Standard mersenne_twister_engine seeded with rd()
static std::uniform_real_distribution<double> equal{0., 1.};

class DNA{
    
public:
    
    DNA(int paramSize, bool randomize);
    double fitness;
    std::vector<int> genes;
    int numberOfGenes;
    int count = 0;
    void fitnessFunction(const std::vector<double>& target);
 
    
    // exponential fitting of score to fitness function to accentuate difference between a slightly better
    //result and its inferior
    DNA& crossover(const DNA& partner);
    
    void mutate(double mutationRate, double eta);
    
    int static polynomialMutate(int originalValue, double index){
       
        //calculate L: the total number of bits used to represent all n variables
        
        //first we get u: a random number between 0 and 1
        double u = equal(engine);
        //calculate parameters dsubl and dsubr
        double newValue= 0;
        
        
        //dsubl
        if(u<= 0.5){
            
            double sublExponent= 1./(1.+ index);
            double dsubl= (pow((2.*u), sublExponent)) - 1.;
            newValue = originalValue + dsubl * (originalValue - 1.); //20 is assumed to be the lower bound a.k.a x sub i ° l
            newValue = floor(newValue);
        }
        
        else{


        double subrExponent = 1./(1. + index);
        double dsubr= 1. - pow((2.*(1.-u)), subrExponent);
        newValue = originalValue + dsubr*(255. - originalValue); //100 is assumed to be the upper bound a.k.a x sub i ^ u
        newValue = floor(newValue);
        }
        
        return newValue;
    }
        
    
    void displayGenes();
};


