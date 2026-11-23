/*
 * Temperature.c
 *
 *  Created on: Feb 18, 2025
 *      Author: Daniel
 */

module;
#include <cstdint>
#include <cstdio>
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
        hw_init_bias_regulator();
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

    void Bias_calibration::hw_init_bias_regulator() {
        // HAL_GPIO_WritePin(VBIAS_CTRL_GPIO_Port, VBIAS_CTRL_Pin, GPIO_PIN_SET); //turn on VBIAS
    }

    void Bias_calibration::print_status() const {
        const auto temperature = Unit_converter::centidegrees_to_parts(get_temperature());
        printf(", temp=%s%d.%02d, bias=%d",
               temperature.sign, temperature.degrees, temperature.hundredths, the_bias_volt);
    }

    void Bias_calibration::print_diag() {
        const auto temperature = Unit_converter::centidegrees_to_parts(get_temperature());
        printf("Bias: 25C=%d, coeff=%d, temp=%s%d.%02d, cal=%d, fail=%d\r\n",
               the_bias_25C, the_temp_coeff, temperature.sign, temperature.degrees, temperature.hundredths,
               cnt_bias_cal, cnt_bias_failed);
        the_DAC.print_diag();
        the_ADC.print_diag();
    }

    int32_t Bias_calibration::calibrateADC(int32_t raw, int32_t BIAScal35, int32_t BIAScal45, int32_t ADC_cal35,
                                           int32_t ADC_cal45) {
        double m = ((double) (BIAScal45 - BIAScal35)) / ((double) (ADC_cal45 - ADC_cal35));
        // slope: 10000/9970 ≈ 1.00301
        return (int32_t) (BIAScal45 + m * (raw - ADC_cal45));
    }

    int32_t the_bias_offset = 0;
    int32_t uart_offset = 0;

    void Bias_calibration::bias_temperature_control() {
        //read temperature sensor (divide by 100 to get Celsius)
        const int16_t temp = 2000;
        // if (STTS22H_ReadTemperatureOneShot(&tempo) != HAL_OK) {
        //     // Error handling (for example, blink an LED)
        //     the_temperature = -200;
        //     return;
        // }
        the_temperature = temp;
        auto temp_float = static_cast<float>(the_temperature) / 100.0f;
        if (calibrate_bias(temp_float).success()) {
            cnt_bias_cal++;
        } else {
            cnt_bias_failed++;
        }
    }

    int32_t Bias_calibration::temperature_to_bias(const float temperature) {
        constexpr double factor_A{0.0001582376};
        constexpr double factor_B{0.0130634082};
        constexpr double factor_C{40.2992234430};
        auto bias = -1000.0 * (factor_A * temperature * temperature + factor_B * temperature + factor_C);
        return static_cast<int32_t>(bias);
    }

    Status_code Bias_calibration::calibrate_bias(const float temperature) {
        enum { Volt_50mV = 5, Delay_10ms = 10, Iterations = 20, Max_offset = 6000, Min_offset = -3000 };
        int32_t wanted_bias = 0;
        if (the_bias_25C != 0) {
            wanted_bias = the_bias_25C + uart_offset;
        } else {
            wanted_bias = temperature_to_bias(temperature);
        }
        the_bias_volt = the_ADC.read_ADC_mV();
        for (int i = 0; i < Iterations; i++) {
            auto success = the_DAC.set_DAC_mV(wanted_bias + the_bias_offset);
            // HAL_Delay(Delay_10ms);
            the_bias_volt = the_ADC.read_ADC_mV();
            // Compare VBias with wanted VBias
            if (the_bias_volt < wanted_bias - Volt_50mV && the_bias_offset <= Max_offset) {
                // When VBias is lower increase VBias
                the_bias_offset += Volt_50mV;
            } else if (the_bias_volt > wanted_bias + Volt_50mV && the_bias_offset >= Min_offset) {
                // When VBias is higher decrease VBias
                the_bias_offset -= Volt_50mV;
            } else {
                return success;
            }
        }
        return Status_code::Failure();
    }
}
