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
* @file   Bias_ADC.h
* @author AndersEmilOlsen, IDEAS
* @date   18.03.2026
* @brief  
*/

#pragma once
#include <cstdint>

namespace Calibration {
    class Bias_ADC {
        int32_t the_ADC_cal_val_35{-35000};
        int32_t the_ADC_cal_val_45{-40700};
        double ADC_to_mV_A{};
        double ADC_to_mV_B{};
        static volatile int32_t the_last_sense;
        bool is_initialized{};

    public:
        void initialize();

        void print_diag();

        void calibrate();

        [[nodiscard]] int32_t read_mV();

        static int32_t get_last_sense() { return the_last_sense; }

        static int32_t read_ADC();

        void set_cal_35V(int32_t cal) { the_ADC_cal_val_35 = cal; }

        void set_cal_45V(int32_t cal) { the_ADC_cal_val_45 = cal; }

    private:
        [[nodiscard]] int32_t to_mV(int32_t ADC) const;

        static int32_t convertADCtoSensorValue(uint16_t adc_value);
    };
} // Temperature
