/*
 * Temperature.c
 *
 *  Created on: Feb 18, 2025
 *      Author: Daniel
 */

module;
#include <cstdint>
#include <cstdio>
#include <cmath>

#include "Persistent_parameter_id.h"

module Component.Bias_calibration;
import Component.Bias_ADC;
import Component.Bias_DAC;
import Support.Configuration_repository;
import Utility.Unit_converter;

namespace Calibration {
    uint32_t cnt_bias_cal{};
    uint32_t cnt_bias_failed{};

    volatile int16_t the_temperature;
    volatile int32_t the_bias_volt;

#define DAC_MAX 4096
#define DAC_MIN 0

    void Bias_calibration::initialize() {
        the_DAC.initialize();
        the_ADC.initialize();
    }

    Status_code Bias_calibration::update_setpoints(Repository::Configuration_repository &repository) {
        auto success{true};
        int32_t DAC_low;
        int32_t DAC_high;
        if (repository.get(Application::Cal_DAC_low, DAC_low).success() &&
            repository.get(Application::Cal_DAC_high, DAC_high).success()) {
            the_DAC.set_setpoints(DAC_low, DAC_high);
        } else {
            success = false;
        }
        int32_t ADC_low;
        int32_t ADC_high;
        if (repository.get(Application::Cal_ADC_low, ADC_low).success() &&
            repository.get(Application::Cal_ADC_high, ADC_high).success()) {
            the_ADC.set_setpoints(ADC_low, ADC_high);
        } else {
            success = false;
        }
        int32_t volt_low;
        int32_t volt_high;
        if (repository.get(Application::Cal_volt_low, volt_low).success() &&
            repository.get(Application::Cal_volt_high, volt_high).success()) {
            the_DAC.set_setpoint_volt(volt_low, volt_high);
            the_ADC.set_setpoint_volt(volt_low, volt_high);
        } else {
            success = false;
        }
        int32_t bias_25C;
        int32_t temp_coeff;
        if (repository.get(Application::Cal_bias_voltage_25C, bias_25C).success() &&
            repository.get(Application::Cal_temperature_coefficient, temp_coeff).success()) {
            the_bias_25C = bias_25C;
            the_temp_coeff = temp_coeff;
        } else {
            success = false;
        }
        return Status_code(success);
    }

    void Bias_calibration::print_status() const {
        const auto temperature = Unit_converter::centidegrees_to_parts(get_temperature());
        printf(", temp=%s%d.%02d, bias=%d",
               temperature.sign, temperature.degrees, temperature.hundredths, the_bias_actual_volt);
    }

    void Bias_calibration::print_diag() {
        print_bias();
        int32_t low_volt;
        int32_t high_volt;
        the_ADC.get_setpoint_volt(low_volt, high_volt);
        printf("Cal: 25C=%dmV, coeff=%dmV/C, low-volt=%dmV, high-volt=%dmV\r\n",
               static_cast<int32_t>(round(the_bias_25C)),
               static_cast<int32_t>(round(the_temp_coeff)),
               low_volt, high_volt);
        the_DAC.print_diag();
        the_ADC.print_diag();
        printf("Cnt: cal=%d, fail=%d\r\n", cnt_bias_cal, cnt_bias_failed);
    }

    void Bias_calibration::print_bias() const {
        const auto temperature = Unit_converter::centidegrees_to_parts(get_temperature());
        printf("Bias: temp=%s%d.%02dC, set=%dmV, volt=%dmV, error=%dmV\r\n",
               temperature.sign, temperature.degrees, temperature.hundredths,
               get_setpoint(),
               get_voltage(),
               get_error());
    }

    void Bias_calibration::get_temperature_factors(int32_t &bias_25C, int32_t &coeff) const {
        bias_25C = static_cast<int32_t>(round(the_bias_25C));
        coeff = static_cast<int32_t>(round(the_temp_coeff));
    }

    int32_t Bias_calibration::get_25C_setpoint() const {
        return static_cast<int32_t>(round(the_bias_25C));
    }

    void Bias_calibration::bias_temperature_control(const int32_t temperature) {
        the_temperature = temperature;
        auto temp_float = static_cast<double>(the_temperature) / 100.0f;
        if (calibrate_bias(temp_float).success()) {
            cnt_bias_cal++;
        } else {
            cnt_bias_failed++;
        }
    }

    int32_t Bias_calibration::temperature_to_bias(const double temperature) const {
        const auto bias = the_bias_25C + the_temp_coeff * (temperature - 25.0);
        return static_cast<int32_t>(bias);
    }

    Status_code Bias_calibration::calibrate_bias(const double temperature) {
        the_bias_set_volt = temperature_to_bias(temperature);
        const auto status = the_DAC.set_DAC_mV(the_bias_set_volt);
        the_bias_actual_volt = the_ADC.read_ADC_mV();
        the_bias_error = the_bias_set_volt - the_bias_actual_volt;
        return status;
        // return regulator_loop(bias);
    }

    Status_code Bias_calibration::regulator_loop(const int32_t wanted_bias) {
        int32_t the_bias_offset = 0;
        enum { Volt_50mV = 5, Delay_10ms = 10, Iterations = 20, Max_offset = 6000, Min_offset = -3000 };
        the_bias_actual_volt = Bias_calibration::the_ADC.read_ADC_mV();
        for (int i = 0; i < Iterations; i++) {
            the_bias_set_volt = wanted_bias + the_bias_offset;
            const auto success = the_DAC.set_DAC_mV(the_bias_set_volt);
            // HAL_Delay(Delay_10ms);
            the_bias_actual_volt = the_ADC.read_ADC_mV();
            // Compare VBias with wanted VBias
            if (the_bias_actual_volt < wanted_bias - Volt_50mV && the_bias_offset <= Max_offset) {
                // When VBias is lower increase VBias
                the_bias_offset += Volt_50mV;
            } else if (the_bias_actual_volt > wanted_bias + Volt_50mV && the_bias_offset >=
                       Min_offset) {
                // When VBias is higher decrease VBias
                the_bias_offset -= Volt_50mV;
            } else {
                return success;
            }
        }
        return Status_code::Failure();
    }
}
