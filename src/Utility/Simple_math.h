//
// Created by anolsen on 08.01.2020.
//

#ifndef UTILITY_SIMPLE_MATH_H
#define UTILITY_SIMPLE_MATH_H

#include <stdint-gcc.h>

namespace Simple_math {
    double square_root(double a);

    double standard_deviation(double sum, double sum_of_squares, double number_of_samples);

    inline void moving_average(const float in[], float out[], const int size, const int window) {
        for (int i = 0; i < size; i++) {
            int start = i - window;
            int end = i + window;
            if (start < 0) start = 0;
            if (end >= size) end = size - 1;
            float sum = 0.0;
            int count = 0;
            for (int j = start; j <= end; j++) {
                sum += in[j];
                count++;
            }
            out[i] = sum / static_cast<float>(count);
        }
    }

    inline void derivative(const float x[], const float y[], float d[], const int size) {
        for (int i = 0; i < size - 1; i++) {
            d[i] = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
        }
        d[size - 1] = d[size - 2];
    }

    inline void proportional_derivative(const float x[], const float y[], float d[], const int size) {
        for (int i = 0; i < size - 1; i++) {
            const auto current = y[i];
            const auto difference = y[i + 1] - current;
            const auto distance = x[i + 1] - x[i];
            if (distance > 0.0 && (current > 0.0 || current < 0.0)) {
                const auto derivative = difference / distance;
                d[i] = derivative / current;
            } else {
                d[i] = 0.0;
            }
        }
        d[size - 1] = d[size - 2];
    }

    inline float mean(const float arr[], const int start, const int end) {
        float sum = 0.0;
        for (int i = start; i < end; i++) {
            sum += arr[i];
        }
        return sum / static_cast<float>(end - start);
    }
}

#endif //UTILITY_SIMPLE_MATH_H
