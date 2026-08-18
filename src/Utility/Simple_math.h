//
// Created by anolsen on 08.01.2020.
//

#ifndef UTILITY_SIMPLE_MATH_H
#define UTILITY_SIMPLE_MATH_H

#include <stdint-gcc.h>

namespace Simple_math {
    double square_root(double a);

    double standard_deviation(double sum, double sum_of_squares, double number_of_samples);
}

#endif //UTILITY_SIMPLE_MATH_H
