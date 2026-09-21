/*
* Copyright (C) 2026 Integrated Detector Electronics AS
* All Rights Reserved.
*
* NOTICE: All information contained herein is, and remains
* the property of Integrated Detector Electronics AS and its suppliers,
* if any. The intellectual and technical concepts contained
* herein are proprietary to Integrated Detector Electronics AS
* and its suppliers and may be covered by Norwegian, EU. or U.S. patents,
* patents in process, and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Integrated Detector Electronics AS.
*/

/**
* @file   Calibration_analyzer.cpp
* @author AndersEmilOlsen, IDEAS
* @date   26.06.2026
* @brief  
*/

module;
#include <cstdint>
#include <cmath>
#include "Simple_math.h"

module Component.Calibration_analyzer;

namespace Calibration {
    uint32_t Calibration_analyzer::find_noise_floor_A(const int32_t *data) {
        int32_t noise_floor{Array_size};
        for (noise_floor = Array_size - Window_size; noise_floor > 0; --noise_floor) {
            auto sum = *(data + noise_floor) + *(data + noise_floor + 1) + *(data + noise_floor + 2);
            if (sum > Max_sum) {
                break;
            }
        }
        return noise_floor + Threshold_margin;
    }

    float Calibration_analyzer::the_temp_x[Array_size]{};
    float Calibration_analyzer::the_temp_y[Array_size]{};

    uint32_t Calibration_analyzer::find_noise_floor_B(const int32_t *data) {
        uint32_t threshold{};
        for (uint32_t index = 0; index < Array_size; index++) {
            the_temp_x[index] = static_cast<float>(index);
            the_temp_y[index] = static_cast<float>(*data++);
        }
        threshold = find_threshold(the_temp_x, the_temp_y);
        return threshold + Threshold_margin;
    }

    uint32_t Calibration_analyzer::find_threshold(const float x[Array_size], const float y[Array_size]) {
        float y_smooth[Array_size];
        float d[Array_size];
        float d_smooth[Array_size];
        // Step 1: smooth signal
        Simple_math::moving_average(y, y_smooth, Array_size, Window_size);
        // // Step 2: derivative
        // Simple_math::derivative(x, y_smooth, d, Array_size);
        // Step 2: proportional derivative
        Simple_math::proportional_derivative(x, y_smooth, d, Array_size);
        // Step 3: smooth derivative
        Simple_math::moving_average(d, d_smooth, Array_size, Window_size);
        // Step 4: find threshold
        uint32_t threshold_index{};
        float max_derivative = 0.0;
        for (size_t i = Array_size; i > 0; --i) {
            const auto derivative = std::fabs(d_smooth[i]);
            if (derivative > max_derivative) {
                threshold_index = i;
                max_derivative = derivative;
            }
        }
        // Step 5: compute noise floor
        // auto noise_floor = mean(y, threshold_index, size);
        return threshold_index;
    }
}
