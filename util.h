//
//  util.cpp
//  min-devkit
//
//  Created by Camilo Ocampo on 12.05.20.
//
#pragma once


namespace utilities {
static double map(long double value, long double start1, long double stop1,
                 long double start2, long double stop2) {
    long double slope = (long double)1.0 * (stop2-start2)/ (stop1- start1);
    long double outgoing = start1 + slope * (value - start1);
        

    return outgoing;
    }
}
