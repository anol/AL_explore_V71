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
* @file   Bias_ADC.cpp
* @author AndersEmilOlsen, IDEAS
* @date   18.03.2026
* @brief  
*/

module;
#include <cmath>
#include <cstdint>
#include <cstdio>

module Component.Bias_ADC;

namespace Calibration {
    volatile int32_t Bias_ADC::the_last_sense{};

    void Bias_ADC::print_diag() {
        const uint32_t ADC_raw{read_ADC_raw()};
        const int32_t ADC_mV{to_mV(ADC_raw)};
        printf("ADC: raw=%d, mV=%d, low-set=%d, high-set=%d, A=%f, B=%f\r\n",
               ADC_raw, ADC_mV,
               static_cast<uint32_t>(round(the_low_setpoint)),
               static_cast<uint32_t>(round(the_high_setpoint)),
               the_ADC_to_mV_A, the_ADC_to_mV_B);
    }

    void Bias_ADC::calibrate() {
        the_ADC_to_mV_A = (the_low_setpoint - the_high_setpoint) / (the_low_volt - the_high_volt);
        the_ADC_to_mV_B = the_high_setpoint - the_ADC_to_mV_A * (the_high_volt);
    }

    int32_t Bias_ADC::read_ADC_mV() {
        uint32_t value;
        use_ADC.get(value);
        return to_mV(value);
    }

    int32_t Bias_ADC::to_mV(const uint32_t ADC) const {
        auto bias_volt = the_ADC_to_mV_A * ADC + the_ADC_to_mV_B;
        return static_cast<int32_t>(bias_volt);
    }
} // Temperature
