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
* @file   Calibration_analyzer.h
* @author AndersEmilOlsen, IDEAS
* @date   02.06.2026
* @brief
*/

module;
#include <cstdint>

export module Component.Calibration_analyzer;



export namespace Calibration {
    class Calibration_analyzer {
        enum { Threshold_margin = 2, Window_size = 3, Max_sum = 20, Array_size = 256 };

        static float the_temp_x[Array_size];
        static float the_temp_y[Array_size];

    public:
        static uint32_t find_noise_floor_A(const int32_t *data);

        static uint32_t find_noise_floor_B(const int32_t *data);

    private:
        static uint32_t find_threshold(const float x[Array_size], const float y[Array_size]);
    };
} // Calibration
