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
* @file   Instrument_calibration_provider.cpp
* @author AndersEmilOlsen, IDEAS
* @date   21.05.2026
* @brief  
*/

module;

#include <cstdint>
#include <cstdio>

#include "Persistent_parameter_id.h"

module Application.Instrument_calibration_provider;
import Component.IDE3380_test_pedestal;
import Component.IDE3380_test_noise_floor;
import Component.Histogram_storage;
import Component.IDE3380_provider;
import Component.Bias_calibration;
import Component.Event_counter;
import Domain.Keyword_lookup;
import Application.AstraLEO_command_lookup;
import Application.AstraLEO_provider_indication;
import Support.Configuration_repository;
import Support.Instruction_major;
import Utility.Unit_converter;
import Platform.Diagnostic;

using namespace IDE3380;
using namespace AstraLEO_interface;
using namespace Application;

/// @brief Show setpoint, voltage and error.
void Instrument_calibration_provider::v_BIAS(Instruction_major &instruction) {
    const auto temperature = Unit_converter::centidegrees_to_parts(use_bias.get_temperature());
    printf("Bias: C25=%dmV, temperature=%s%d.%02dC, setpoint=%dmV, voltage=%dmV, error=%dmV\r\n",
           use_bias.get_C25_setpoint(),
           temperature.sign, temperature.degrees, temperature.hundredths,
           use_bias.get_setpoint(),
           use_bias.get_voltage(),
           use_bias.get_error());
    instruction.print_ack();
}

/// @brief Get the raw ADC reading.
void Instrument_calibration_provider::v_BIAS_ADC(Instruction_major &instruction) {
    auto ADC_raw = use_bias.get_ADC().read_ADC_raw();
    instruction.print_ack(static_cast<int32_t>(ADC_raw));
}

/// @brief Get the ADC -30V and -40V calibration points.
void Instrument_calibration_provider::v_BIAS_ADC_SET(Instruction_major &instruction) {
    int32_t low{};
    int32_t high{};
    use_bias.get_ADC().get_setpoints(low, high);
    instruction.print_ack(low, high);
}

/// @brief Set the ADC -30V and -40V calibration points.
void Instrument_calibration_provider::v_BIAS_ADC_SET_low_high(Instruction_major &instruction, const int low_2,
                                                              const int high_3) {
    use_bias.get_ADC().set_setpoints(low_2, high_3);
    if (use_repository.set(Cal_ADC_low, low_2).success() && use_repository.set(Cal_ADC_high, high_3).success()) {
        instruction.print_ack(low_2, high_3);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

/// @brief Get the 25C bias setpoint.
void Instrument_calibration_provider::v_BIAS_C25(Instruction_major &instruction) {
    const auto bias{use_bias.get_C25_setpoint()};
    instruction.print_ack(bias);
}

/// @brief Set the 25C bias setpoint. 0=use temperature.
void Instrument_calibration_provider::v_BIAS_C25_bias_mV(Instruction_major &instruction, const int bias_mV_2) {
    use_bias.set_C25_setpoint(bias_mV_2);
    if (use_repository.set(Cal_bias_voltage_25C, bias_mV_2).success()) {
        if (bias_mV_2 == 0) {
            instruction.print_ack(bias_mV_2);
            // Trigger setpoint update by temperature
            use_bias.bias_temperature_control();
        } else {
            // Override the internal bias setpoint (allowed in any mode as part of setting C25).
            override_setpoint(instruction, bias_mV_2);
        }
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

/// @brief Set the calibration point voltages (e.g. -35000mV and -45000mV).
void Instrument_calibration_provider::v_BIAS_CAL_low_mV_high_mV(Instruction_major &instruction, int low_mV_2, int high_mV_3) {
    use_bias.set_setpoint_volt(low_mV_2, high_mV_3);
    if (use_repository.set(Cal_volt_low, low_mV_2).success() && use_repository.set(Cal_volt_high, high_mV_3).success()) {
        instruction.print_ack(low_mV_2, high_mV_3);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

/// @brief Get the calibration point voltages.
void Instrument_calibration_provider::v_BIAS_CAL(Instruction_major &instruction) {
    int32_t low_mV{};
    int32_t high_mV{};
    use_bias.get_setpoint_volt(low_mV, high_mV);
    instruction.print_ack(low_mV, high_mV);
}

/// @brief Misc. diagnostic information wrt. bias voltage.
void Instrument_calibration_provider::v_BIAS_DIAG(Instruction_major &instruction) {
    use_bias.print_diag();
    instruction.print_ack();
}

/// @brief Get the bias voltage error.
void Instrument_calibration_provider::v_BIAS_ERROR(Instruction_major &instruction) {
    const auto error = use_bias.get_error();
    instruction.print_ack(error);
}

/// @brief Get the temp. compensated bias setpoint.
void Instrument_calibration_provider::v_BIAS_SET(Instruction_major &instruction) {
    const auto setpoint = use_bias.get_setpoint();
    instruction.print_ack(setpoint);
}

/// @brief Override the temp. compensated bias setpoint.
void Instrument_calibration_provider::v_BIAS_SET_bias_mV(Instruction_major &instruction, const int bias_mV_2) {
    if (require_calibration_mode(instruction)) {
        override_setpoint(instruction, bias_mV_2);
    }
}

void Instrument_calibration_provider::override_setpoint(Instruction_major &instruction, const int bias_mV_2) {
    if (use_bias.override_setpoint(bias_mV_2)) {
        instruction.print_ack(bias_mV_2);
    } else {
        instruction.print_nack(714);
    }
}

/// @brief Get the temperature.
void Instrument_calibration_provider::v_BIAS_TEMP(Instruction_major &instruction) {
    const auto temperature = use_bias.get_temperature();
    instruction.print_ack(temperature);
}

/// @brief Get the temperature correction.
void Instrument_calibration_provider::v_BIAS_TEMP_SET(Instruction_major &instruction) {
    int32_t a{};
    int32_t b{};
    use_bias.get_temperature_factors(a, b);
    instruction.print_ack(a, b);
}

/// @brief OVERRIDE the temperature reading (C/100)
void Instrument_calibration_provider::v_BIAS_TEMP_temp(Instruction_major &instruction, int temp_2) {
    if (require_calibration_mode(instruction)) {
        use_housekeeper.get_temperature_sensor().override_temperature(temp_2);
        instruction.print_ack(temp_2);
    }
}

/// @brief Set the temperature correction factors.
void Instrument_calibration_provider::v_BIAS_TEMP_SET_a_b(Instruction_major &instruction, const int a_3,
                                                          const int b_4) {
    use_bias.set_temperature_factors(a_3, b_4);
    instruction.print_ack(a_3, b_4);
}

/// @brief Get the bias voltage.
void Instrument_calibration_provider::v_BIAS_VOLT(Instruction_major &instruction) {
    const auto bias{use_bias.get_voltage()};
    instruction.print_ack(bias);
}

/// @brief Get the raw DAC setting.
void Instrument_calibration_provider::v_BIAS_DAC(Instruction_major &instruction) {
    instruction.print_ack(static_cast<int32_t>(use_bias.get_DAC().get_DAC_raw()));
}

/// @brief Override the DAC setting.
void Instrument_calibration_provider::v_BIAS_DAC_data12(Instruction_major &instruction, const int data12_2) {
    enum { DAC_min = 0, DAC_max = 4095 }; // 12-bit DAC; out-of-range values would wrap on hardware
    if (!require_calibration_mode(instruction)) {
        // NACK'ed
    } else if (data12_2 < DAC_min || data12_2 > DAC_max) {
        instruction.print_nack(923);
    } else if (use_bias.get_DAC().set_DAC_raw(static_cast<uint32_t>(data12_2))) {
        instruction.print_ack(data12_2);
    } else {
        instruction.print_nack(723);
    }
}

/// @brief Get the DAC -30V and -40V calibration points.
void Instrument_calibration_provider::v_BIAS_DAC_SET(Instruction_major &instruction) {
    int32_t low{};
    int32_t high{};
    use_bias.get_DAC().get_setpoints(low, high);
    instruction.print_ack(low, high);
}

/// @brief Set the DAC -30V and -40V calibration points.
void Instrument_calibration_provider::v_BIAS_DAC_SET_low_high(Instruction_major &instruction, const int low_3,
                                                              const int high_4) {
    use_bias.get_DAC().set_setpoints(low_3, high_4);
    if (use_repository.set(Cal_DAC_low, low_3).success() && use_repository.set(Cal_DAC_high, high_4).success()) {
        instruction.print_ack(low_3, high_4);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

/// @brief Find common bias voltage using dark count rate.
void Instrument_calibration_provider::v_CAL_DUMP(Instruction_major &instruction) {
    if (require_calibration_mode(instruction)) {
        the_test_pedestal.print_pedestals();
        instruction.print_ack();
    }
}

/// @brief Find common bias voltage using dark count rate.
void Instrument_calibration_provider::v_CAL_DIAG(Instruction_major &instruction) {
    if (require_calibration_mode(instruction)) {
        the_test_pedestal.print_diag();
        the_test_noise_floor.print_diag();
        instruction.print_ack();
    }
}

/// @brief Find common bias voltage using dark count rate.
void Instrument_calibration_provider::v_CAL_TEST_BIAS(Instruction_major &instruction) {
    if (require_calibration_mode(instruction)) {
        instruction.print_nack("NOT IMPLEMENTED");
    }
}

/// @brief Find channel offset voltage using fixed source.
void Instrument_calibration_provider::v_CAL_TEST_OFFSET(Instruction_major &instruction) {
    if (require_calibration_mode(instruction)) {
        instruction.print_nack("NOT IMPLEMENTED");
    }
}

/// @brief Find channel noise floor using threshold scan.
void Instrument_calibration_provider::v_CAL_TEST_NOISE(Instruction_major &instruction) {
    if (require_calibration_mode(instruction)) {
        int32_t integration_time{};
        int32_t start_threshold{};
        int32_t stop_count{};
        if (use_repository.get(Cal_integration_time, integration_time).success() &&
            use_repository.get(Cal_start_threshold, start_threshold).success() &&
            use_repository.get(Cal_stop_count, stop_count).success()) {
            the_test_noise_floor.set_attributes(integration_time, start_threshold, stop_count);
        }
        the_test_noise_floor.set_trace(the_trace_flag);
        if (the_test_noise_floor.start_test(&instruction).success()) {
            instruction.print_pending();
        } else {
            instruction.print_nack(the_test_pedestal.get_diag_code());
        }
    }
}

/// @brief Find channel pedestal using forced readout.
void Instrument_calibration_provider::v_CAL_TEST_PEDESTAL(Instruction_major &instruction) {
    if (require_calibration_mode(instruction)) {
        the_test_pedestal.set_trace(the_trace_flag);
        if (the_test_pedestal.start_test(&instruction).success()) {
            instruction.print_pending();
        } else {
            instruction.print_nack(the_test_pedestal.get_diag_code());
        }
    }
}

/// @brief Toggle diagnostic trace on/off.
void Instrument_calibration_provider::v_CAL_TRACE(Instruction_major &instruction) {
    if (require_calibration_mode(instruction)) {
        the_trace_flag = !the_trace_flag;
        printf("The calibration trace is %s\r\n", the_trace_flag ? "ON" : "OFF");
        instruction.print_ack();
    }
}

bool Instrument_calibration_provider::require_calibration_mode(Instruction_major &instruction) const {
    if (use_mode_control.is_calibration_mode()) {
        return true;
    }
    instruction.print_nack("NOT IN CALIBRATION MODE");
    return false;
}
