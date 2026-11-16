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

module;
#include <cstdint>
import Type.Abstract_ADC;

export module Component.Bias_ADC;


export namespace Calibration {
    class Bias_ADC {
        Abstract::Abstract_ADC &use_ADC;
        int32_t the_v30_setpoint{2048};
        int32_t the_v40_setpoint{1024};
        double ADC_to_mV_A{};
        double ADC_to_mV_B{};
        static volatile int32_t the_last_sense;

    public:
        explicit Bias_ADC(Abstract::Abstract_ADC &ADC) : use_ADC(ADC) {
        }

        void initialize();

        void print_diag();

        void calibrate();

        [[nodiscard]] int32_t read_mV();

        static int32_t get_last_sense() { return the_last_sense; }

        int32_t read_ADC();

        [[nodiscard]] bool read_raw_ADC(uint32_t &value) { return use_ADC.get(value); }

        void set_setpoints(const int32_t v30, const int32_t v40) {
            the_v30_setpoint = v30;
            the_v40_setpoint = v40;
        }

        void get_setpoints(int32_t &v30, int32_t &v40) const {
            v30 = the_v30_setpoint;
            v40 = the_v40_setpoint;
        }

    private:
        [[nodiscard]] int32_t to_mV(int32_t ADC) const;

        static int32_t convertADCtoSensorValue(uint16_t adc_value);
    };
} // Temperature
