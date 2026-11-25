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

    void Bias_DAC::initialize() const {
        enum { Initial_DAC_setting = 0, };
        use_DAC.set(Initial_DAC_setting);
    }

    void Bias_DAC::print_diag() const {
        const auto DAC_raw = use_DAC.get();
        printf("DAC: raw=%d, mV=%d, low-set=%d, high-set=%d\r\n",
               DAC_raw, the_DAC_mV,
               static_cast<uint32_t>(round(the_low_setpoint)),
               static_cast<uint32_t>(round(the_high_setpoint)));
    }

    uint32_t Bias_DAC::mV_to_DAC(double bias_mV) const {
        const auto bias_range = static_cast<double>(the_high_volt + the_low_volt);
        const auto DAC_range = static_cast<double>(the_high_setpoint - the_low_setpoint);
        const auto coeff = DAC_range / bias_range;
        const auto offset = coeff * (the_high_volt - bias_mV);
        const auto DAC_value = static_cast<uint32_t>(the_high_setpoint + offset);
        return DAC_value;
    }

    Status_code Bias_DAC::set_DAC_mV(const int32_t bias_mV) {
        the_DAC_mV = bias_mV;
        if (bias_mV > -30000 || bias_mV < -48000) {
            cnt_out_of_range++;
            return Status_code::Failure();
        }
        if (in_calibration) {
            cnt_busy_in_cal++;
            return Status_code::Failure();
        }
        auto DAC_raw = mV_to_DAC(bias_mV);
        use_DAC.set(DAC_raw);
        return Status_code::Success();
    }
} // Calibration
