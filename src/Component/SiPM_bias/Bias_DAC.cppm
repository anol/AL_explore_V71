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
        uint32_t the_v30_setpoint{2048};
        uint32_t the_v40_setpoint{1024};
        int32_t the_DAC_pre_cal{};
        static volatile uint32_t the_DAC_setting;

    public:
        explicit Bias_DAC(Abstract::Abstract_DAC &DAC) : use_DAC(DAC) {
        }

        void initialize();

        void print_diag();

        int32_t calibrate(int32_t raw, int32_t cal35, int32_t cal45);

        [[nodiscard]] uint32_t get_raw_DAC() const { return use_DAC.get(); }

        [[nodiscard]] bool set_raw_DAC(const uint32_t value) const { return use_DAC.set(value); }

        void set_dac_test(int32_t value);

        void set_dac_cal_test(int32_t value);

        [[nodiscard]] Status_code set_dac_value_with_calibration(int32_t value);

        void set_setpoints(const int32_t v30, const int32_t v40) {
            the_v30_setpoint = v30;
            the_v40_setpoint = v40;
        }

        void get_setpoints(int32_t &v30, int32_t &v40) const {
            v30 = the_v30_setpoint;
            v40 = the_v40_setpoint;
        }

        [[nodiscard]] int32_t get_pre_cal() const { return the_DAC_pre_cal; }

        [[nodiscard]] uint32_t get_DAC_setting() const { return the_DAC_setting; }

        static int32_t get_last_bias() { return the_DAC_setting; };
    };
} // Calibration
