/*
 * Temperature.h
 *
 *  Created on: Feb 18, 2025
 *      Author: Daniel
 */

#pragma once

#include "Bias_ADC.h"
#include "Bias_DAC.h"

namespace Repository {
    class Configuration_repository;
}

namespace Calibration {
    class Bias_calibration {
        int32_t the_bias_at_25{};
        volatile int16_t the_temperature{};
        volatile int32_t the_bias_volt{};
        float gamma_param_A = 1;
        float gamma_param_B = 0;
        Bias_DAC the_DAC{};
        Bias_ADC the_ADC{};

    public:
        void initialize();

        void bias_temperature_control();

        int32_t temperature_to_bias(float temperature);

        [[nodiscard]] Status_code calibrate_bias(float temperature);

        void set_calibration(int32_t DAC_35V, int32_t DAC_45V, int32_t ADC_35V, int32_t ADC_45V, int32_t bias_25C);

        void print_diag();

        int32_t calibrateADC(int32_t raw, int32_t BIAScal35, int32_t BIAScal45, int32_t ADC_cal35, int32_t ADC_cal45);

        Bias_DAC &get_DAC() { return the_DAC; }

        Bias_ADC &get_ADC() { return the_ADC; }

        void set_bias_25C(const int32_t cal) { the_bias_at_25 = cal; }

        [[nodiscard]] int32_t get_temperature() const { return the_temperature; }

        [[nodiscard]] int32_t get_voltage() const { return the_bias_volt; }

        [[nodiscard]] int32_t get_DAC_raw() const { return the_DAC.get_DAC_raw(); }

        [[nodiscard]] uint32_t get_DAC_setting() const { return the_DAC.get_DAC_setting(); }

        [[nodiscard]] Status_code update_setpoints(Repository::Configuration_repository &repository);

        void set_gamma_param_A(const float x) { gamma_param_A = x; }

        void set_gamma_param_B(const float x) { gamma_param_B = x; }

        void print_status() const;

    private:
        static void hw_init_bias_regulator();
    };
}
