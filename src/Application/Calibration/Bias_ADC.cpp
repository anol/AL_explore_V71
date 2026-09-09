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


#include "Calibration/Bias_ADC.h"
#include "adc.h"
#include "Print_support.h"

// Define ADC and calibration parameters
#define ADC_FULL_SCALE   16383.0f  // 14-bit ADC max count
#define VREF             3.3f      // Reference voltage in volts
#define CAL_SLOPE        18018.02f // Slope in mV per volt (derived from calibration)
#define CAL_INTERCEPT    (-56126.13f) // Intercept in mV (derived from calibration)

namespace Calibration {
    constexpr static float constant_35{-35'000.0};
    constexpr static float constant_40{-40'000.0}; // 2026-03-18/Aeo: why is this not -45'000?

    volatile int32_t Bias_ADC::the_last_sense{};

    void Bias_ADC::initialize() {
        HAL_ADC_Init(&hadc1);
        HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);
        is_initialized = true;
    }

    void Bias_ADC::print_diag() {
        // PRINTF("ADC: range=%d, busy=%d\r\n", 0, 0);
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
        uint32_t adc_val = 0;
        HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);
        for (int i = 0; i < 256; i++) {
            HAL_ADC_Start(&hadc1);
            HAL_ADC_PollForConversion(&hadc1, 100);
            adc_val += HAL_ADC_GetValue(&hadc1);
            HAL_ADC_Stop(&hadc1);
        }
        return convertADCtoSensorValue(adc_val / 256);
    }
} // Temperature
