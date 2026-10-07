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
#include <cstdint>
#include <cstdio>

module Component.Bias_ADC;

// Define ADC and calibration parameters
constexpr float ADC_FULL_SCALE = 16383.0f;    // 14-bit ADC max count
constexpr float VREF = 3.3f;                  // Reference voltage in volts
constexpr float CAL_SLOPE = 18018.02f;        // Slope in mV per volt (derived from calibration)
constexpr float CAL_INTERCEPT = (-56126.13f); // Intercept in mV (derived from calibration)

namespace Calibration {
    constexpr static float constant_35{-35'000.0};
    constexpr static float constant_40{-40'000.0}; // 2026-03-18/Aeo: why is this not -45'000?

    volatile int32_t Bias_ADC::the_last_sense{};

    void Bias_ADC::initialize() {
        use_ADC.initialize();
    }

    void Bias_ADC::print_diag() {
        printf("ADC: range=%d, busy=%d\r\n", 0, 0);
    }

    void Bias_ADC::calibrate() {
        ADC_to_mV_A = static_cast<double>(the_ADC_cal_val_35 - the_ADC_cal_val_45) / (constant_35 - constant_40);
        ADC_to_mV_B = (the_ADC_cal_val_45 - ADC_to_mV_A * (constant_40));
    }

    int32_t Bias_ADC::read_mV() {
        auto value = read_ADC();
        the_last_sense = to_mV(value);
        return the_last_sense;
    }

    int32_t Bias_ADC::to_mV(const int32_t ADC) const {
        auto bias_volt = ADC_to_mV_A * ADC + ADC_to_mV_B;
        return static_cast<int32_t>(bias_volt);
    }

    int32_t Bias_ADC::convertADCtoSensorValue(uint16_t adc_value) {
        // Calculate voltage ADC at 1.45V = -30000mV and 0.34V = -50000mV (VADC 14bit 3.3V = 16383)
        // Calculate the voltage measured by the ADC (in volts)
        float V_adc = ((float) adc_value / ADC_FULL_SCALE) * VREF;
        // Calculate the sensor value using the linear calibration equation:
        // sensor_value = (slope * V_adc) + intercept
        float sensor_value = CAL_SLOPE * V_adc + CAL_INTERCEPT;
        // Return the sensor value in mV (rounded)
        return (int32_t) (sensor_value + 0.5f);
    }

    int32_t Bias_ADC::read_ADC() {
        if (uint32_t value{}; use_ADC.get(value)) {
            return convertADCtoSensorValue(value / 256);
        }
        return 0;
    }
} // Temperature
