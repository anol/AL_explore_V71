/*
 * Temperature.c
 *
 *  Created on: Feb 18, 2025
 *      Author: Daniel
 */
#include <stdio.h>

#include "Bias_calibration.h"

#include "Bias_ADC.h"
#include "Bias_DAC.h"
#include "Persistent_parameter_id.h"
#include "Sensor_STTS22H.h"


// #include "usart.h"
#include "Configuration_repository.h"

namespace Calibration
{
    uint32_t cnt_bias_cal{};
    uint32_t cnt_bias_failed{};

    volatile int16_t the_temperature;
    volatile int32_t the_bias_volt;

#define DAC_MAX 4096
#define DAC_MIN 0

    void Bias_calibration::initialize()
    {
        the_DAC.initialize();
        the_ADC.initialize();
        hw_init_bias_regulator();
        //only for testing vbias
        // HAL_UART_Init(&huart1);
        // if (STTS22H_Init() != HAL_OK) {
        //     while (1);
        // }
    }

    void Bias_calibration::set_calibration(
        int32_t DAC_35V, const int32_t DAC_45V, const int32_t ADC_35V, const int32_t ADC_45V, const int32_t bias_25C)
    {
        the_DAC.set_cal_35V(DAC_35V);
        the_DAC.set_cal_45V(DAC_45V);
        the_ADC.set_cal_35V(ADC_35V);
        the_ADC.set_cal_45V(ADC_45V);
        the_bias_at_25 = bias_25C;
    }

    bool Bias_calibration::update_setpoints(Repository::Configuration_repository& repository)
    {
        auto success{true};
        int32_t DAC_35V;
        int32_t DAC_45V;
        int32_t ADC_35V;
        int32_t ADC_45V;
        int32_t bias_25C;
        success = repository.get(Application::Cal_DAC_35V, DAC_35V) ? success : false;
        success = repository.get(Application::Cal_DAC_40V, DAC_45V) ? success : false;
        success = repository.get(Application::Cal_ADC_35V, ADC_35V) ? success : false;
        success = repository.get(Application::Cal_ADC_40V, ADC_45V) ? success : false;
        success = repository.get(Application::Cal_bias_25C, bias_25C) ? success : false;
        if (success)
        {
            set_calibration(DAC_35V, DAC_45V, ADC_35V, ADC_45V, bias_25C);
        }
        int32_t param_A;
        success = repository.get(Application::Cal_parameter_A, param_A) ? success : false;
        set_gamma_param_A(static_cast<float>(param_A) / 1000000.0f);
        int32_t param_B;
        success = repository.get(Application::Cal_parameter_B, param_B) ? success : false;
        set_gamma_param_B(static_cast<float>(param_B) / 1000.0f);
        return success;
    }

    void Bias_calibration::hw_init_bias_regulator()
    {
        // HAL_GPIO_WritePin(VBIAS_CTRL_GPIO_Port, VBIAS_CTRL_Pin, GPIO_PIN_SET); //turn on VBIAS
    }

    void Bias_calibration::print_status() const
    {
        printf(", temp=%d.%d, bias=%d",
              get_temperature() / 100, get_temperature() % 100, the_bias_volt);
    }

    void Bias_calibration::print_diag()
    {
        printf("Temp: temp=%d.%d, bias=%d, DAC(raw)=%d, DAC(cal)=%d, ADC=%d, cal=%d, fail=%d\r\n",
              get_temperature() / 100, get_temperature() % 100, the_bias_volt,
              get_DAC_raw(), get_DAC_setting(), the_ADC.read_ADC(), cnt_bias_cal, cnt_bias_failed);
        the_DAC.print_diag();
        the_ADC.print_diag();
    }

    int32_t Bias_calibration::calibrateADC(int32_t raw, int32_t BIAScal35, int32_t BIAScal45, int32_t ADC_cal35,
                                           int32_t ADC_cal45)
    {
        double m = ((double)(BIAScal45 - BIAScal35)) / ((double)(ADC_cal45 - ADC_cal35));
        // slope: 10000/9970 ≈ 1.00301
        return (int32_t)(BIAScal45 + m * (raw - ADC_cal45));
    }


    int32_t the_bias_offset = 0;
    int32_t uart_offset = 0;


    void Bias_calibration::bias_temperature_control()
    {
        //read temperature sensor (divide by 100 to get Celsius)
        int16_t tempo = 0;
        // if (STTS22H_ReadTemperatureOneShot(&tempo) != HAL_OK) {
        //     // Error handling (for example, blink an LED)
        //     the_temperature = -200;
        //     return;
        // }
        the_temperature = tempo;
        auto temp_float = static_cast<float>(the_temperature) / 100.0f;
        if (calibrate_bias(temp_float))
        {
            cnt_bias_cal++;
        }
        else
        {
            cnt_bias_failed++;
        }
    }

    int32_t Bias_calibration::temperature_to_bias(const float temperature)
    {
        constexpr double factor_A{0.0001582376};
        constexpr double factor_B{0.0130634082};
        constexpr double factor_C{40.2992234430};
        auto bias = -1000.0 * (factor_A * temperature * temperature + factor_B * temperature + factor_C);
        return static_cast<int32_t>(bias);
    }

    bool Bias_calibration::calibrate_bias(const float temperature)
    {
        enum { Volt_50mV = 5, Delay_10ms = 10, Iterations = 20, Max_offset = 6000, Min_offset = -3000 };
        the_ADC.calibrate();
        int32_t wanted_bias = 0;
        if (the_bias_at_25 != 0)
        {
            wanted_bias = the_bias_at_25 + uart_offset;
        }
        else
        {
            wanted_bias = temperature_to_bias(temperature);
        }
        the_bias_volt = the_ADC.read_mV();
        for (int i = 0; i < Iterations; i++)
        {
            auto success = the_DAC.set_dac_value_with_calibration(wanted_bias + the_bias_offset);
            // HAL_Delay(Delay_10ms);
            the_bias_volt = the_ADC.read_mV();
            // Compare VBias with wanted VBias
            if (the_bias_volt < wanted_bias - Volt_50mV && the_bias_offset <= Max_offset)
            {
                // When VBias is lower increase VBias
                the_bias_offset += Volt_50mV;
            }
            else if (the_bias_volt > wanted_bias + Volt_50mV && the_bias_offset >= Min_offset)
            {
                // When VBias is higher decrease VBias
                the_bias_offset -= Volt_50mV;
            }
            else
            {
                return success;
            }
        }
        return false;
    }
}
