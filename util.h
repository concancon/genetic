//
//  util.cpp
//  min-devkit
//
//  Created by Camilo Ocampo on 12.05.20.
//
#pragma once

namespace utilities {
static double map(double value, double start1, double stop1, double start2,
                  double stop2) {
    double slope = (double)1.0 * (stop2 - start2) / (stop1 - start1);
    double outgoing = start1 + slope * (value - start1);

    return outgoing;
}
} // namespace utilities
