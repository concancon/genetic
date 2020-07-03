#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <stdbool.h>
#include <stdint.h>
#include "platform.h"
#include "internals.h"
#include "specialize.h"
#include "softfloat.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef SOFTFLOAT_FAST_INT64

float16_t f128M_to_f16( const float128_t *aPtr )
{

    return f128_to_f16( *aPtr );

}

#else

float16_t f128M_to_f16( const float128_t *aPtr )
{
    const uint32_t *aWPtr;
    uint32_t uiA96;
    bool sign;
    int32_t exp;
    uint32_t frac32;
    struct commonNaN commonNaN;
    uint16_t uiZ, frac16;
    union ui16_f16 uZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    aWPtr = (const uint32_t *) aPtr;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA96 = aWPtr[indexWordHi( 4 )];
    sign = signF128UI96( uiA96 );
    exp  = expF128UI96( uiA96 );
    frac32 =
        fracF128UI96( uiA96 )
            | ((aWPtr[indexWord( 4, 2 )] | aWPtr[indexWord( 4, 1 )]
                    | aWPtr[indexWord( 4, 0 )])
                   != 0);
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( exp == 0x7FFF ) {
        if ( frac32 ) {
            softfloat_f128MToCommonNaN( aWPtr, &commonNaN );
            uiZ = softfloat_commonNaNToF16UI( &commonNaN );
        } else {
            uiZ = packToF16UI( sign, 0x1F, 0 );
        }
        goto uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    frac16 = frac32>>2 | (frac32 & 3);
    if ( ! (exp | frac16) ) {
        uiZ = packToF16UI( sign, 0, 0 );
        goto uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    exp -= 0x3FF1;
    if ( sizeof (int_fast16_t) < sizeof (int32_t) ) {
        if ( exp < -0x40 ) exp = -0x40;
    }
    return softfloat_roundPackToF16( sign, exp, frac16 | 0x4000 );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}

#endif














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


