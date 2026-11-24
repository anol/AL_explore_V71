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
* @file   Bias_DAC.h
* @author AndersEmilOlsen, IDEAS
* @date   19.03.2026
* @brief  
*/

module;
#include <cstdint>

export module Component.Bias_DAC;
import Type.Status_code;
import Type.Abstract_DAC;


export namespace Calibration {
    class Bias_DAC {
        Abstract::Abstract_DAC &use_DAC;
        int32_t the_low_setpoint{2048};
        int32_t the_high_setpoint{1024};
        int32_t the_low_volt{};
        int32_t the_high_volt{};
        int32_t the_DAC_mV{};

    public:
        explicit Bias_DAC(Abstract::Abstract_DAC &DAC) : use_DAC(DAC) {
        }

        void initialize() const;

        void print_diag() const;

        [[nodiscard]] uint32_t get_DAC_raw() const { return use_DAC.get(); }

        [[nodiscard]] bool set_DAC_raw(const uint32_t value) const { return use_DAC.set(value); }

        [[nodiscard]] Status_code set_DAC_mV(int32_t bias_mV);

        [[nodiscard]] int32_t get_DAC_mV() const { return the_DAC_mV; }

        void set_setpoints(const int32_t low, const int32_t high) {
            the_low_setpoint = low;
            the_high_setpoint = high;
        }

        void get_setpoints(int32_t &low, int32_t &high) const {
            low = the_low_setpoint;
            high = the_high_setpoint;
        }

        void set_setpoint_volt(const int32_t low_volt, const int32_t high_volt) {
            the_low_volt = low_volt;
            the_high_volt = high_volt;
        }

        void get_setpoint_volt(int32_t &low_volt, int32_t &high_volt) const {
            low_volt = the_low_volt;
            high_volt = the_high_volt;
        }

    private:
        [[nodiscard]] uint32_t mV_to_DAC(double calibrated_mV) const;
    };
} // Calibration
