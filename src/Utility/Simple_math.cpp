//
// Copied from https://www.codeproject.com/Articles/570700/SquareplusRootplusalgorithmplusforplusC on 08.01.2020.
// Please see here as well: https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi
//

module;
#include <cstdint>

module Utility.Simple_math;
import Utility.Json_printer;

static double powerOfTen(int num) {
    double rst = 1.0;
    if (num >= 0) {
        for (int i = 0; i < num; i++) {
            rst *= 10.0;
        }
    } else {
        for (int i = 0; i < (0 - num); i++) {
            rst *= 0.1;
        }
    }
    return rst;
}

double Simple_math::square_root(double a) {
    double z = a;
    double rst = 0.0;
    int max = 8;     // to define maximum digit
    int i;
    double j = 1.0;
    for (i = max; i > 0; i--) {
        // value must be bigger then 0
        if (z - ((2 * rst) + (j * powerOfTen(i))) * (j * powerOfTen(i)) >= 0) {
            while (z - ((2 * rst) + (j * powerOfTen(i))) * (j * powerOfTen(i)) >= 0) {
                j++;
                if (j >= 10) break;
            }
            j--; //correct the extra value by minus one to j
            z -= ((2 * rst) + (j * powerOfTen(i))) * (j * powerOfTen(i)); //find value of z
            rst += j * powerOfTen(i);     // find sum of a
            j = 1.0;
        }
    }
    for (i = 0; i >= 0 - max; i--) {
        if (z - ((2 * rst) + (j * powerOfTen(i))) * (j * powerOfTen(i)) >= 0) {
            while (z - ((2 * rst) + (j * powerOfTen(i))) * (j * powerOfTen(i)) >= 0) {
                j++;
            }
            j--;
            z -= ((2 * rst) + (j * powerOfTen(i))) * (j * powerOfTen(i)); //find value of z
            rst += j * powerOfTen(i);     // find sum of a
            j = 1.0;
        }
    }
    // find the number on each digit
    return rst;
}

double Simple_math::standard_deviation(double sum, double sum_of_squares, double number_of_samples) {
    double result = 0.0;
    if (number_of_samples > 2.0) {
        double core = (number_of_samples * sum_of_squares - sum * sum) / (number_of_samples * (number_of_samples - 1));
        if (core > 0.0) {
            result = Simple_math::square_root(core);
        }
    }
    return result;
}
