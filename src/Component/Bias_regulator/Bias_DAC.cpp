/*
* Copyright (C) 2020-2025 Integrated Detector Electronics AS
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
* @file   Bias_DAC.cpp
* @author AndersEmilOlsen, IDEAS
* @date   19.03.2026
* @brief  
*/

module;
#include <cmath>
#include <cstdint>
#include <cstdio>

module Component.Bias_DAC;
import Type.Status_code;

// #include "dac.h"

namespace Calibration {
    static uint32_t cnt_out_of_range{};
    static uint32_t cnt_busy_in_cal{};
    static uint8_t in_calibration = 0;

    void Bias_DAC::initialize() {
        enum { Initial_DAC_setting = 0, };
        use_DAC.set(Initial_DAC_setting);
    }

    void Bias_DAC::print_diag() {
        auto DAC_raw = use_DAC.get();
        printf("DAC: raw=%d, mV=%d, low-set=%d, high-set=%d \r\n",
               DAC_raw, the_DAC_mV,
               static_cast<uint32_t>(round(the_low_setpoint)),
               static_cast<uint32_t>(round(the_high_setpoint)));
    }

    int32_t Bias_DAC::calibrate(const int32_t raw, const int32_t cal35, const int32_t cal45) {
        double m = (-40700.0 + 35000.0) / static_cast<double>(cal45 - cal35); // slope: 10000/9970 ≈ 1.00301
        return static_cast<int32_t>(-40700.0 + m * (raw - cal45));
    }

    Status_code Bias_DAC::set_DAC_mV(const int32_t value) {
        the_DAC_mV = value;
        if (value > -30000 || value < -48000) {
            cnt_out_of_range++;
            return Status_code::Failure();
        }
        if (in_calibration) {
            cnt_busy_in_cal++;
            return Status_code::Failure();
        }
        float temp = calibrate(value, the_low_setpoint, the_high_setpoint);
        temp *= -1.0f;
        temp -= 30000.0f;
        temp = (temp * 4095.0f) / 20000.0f;
        if (temp > 4095.0f) {
            temp = 4095.0f;
        }
        if (temp < 0.0f) {
            temp = 0.0f;
        }
        auto DAC_raw = static_cast<uint32_t>(temp);
        use_DAC.set(DAC_raw);
        return Status_code::Success();
    }
} // Calibration
