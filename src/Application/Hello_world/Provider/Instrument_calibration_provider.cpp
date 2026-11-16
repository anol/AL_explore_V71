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
#include "Persistent_parameter_id.h"
#include <cstdint>
#include <cstdio>

module Application.Instrument_calibration_provider;
import Component.IDE3380_test_pedestal;
import Component.IDE3380_test_noise_floor;
import Component.Histogram_storage;
import Component.IDE3380_interface;
import Component.Bias_calibration;
import Component.Event_counter;
import Domain.Keyword_lookup;
import Application.SpectraNode_command_lookup;
import Application.SpectraNode_provider_indication;
import Support.Configuration_repository;
import Support.Instruction_major;

using namespace IDE3380;

using namespace SpectraNode_interface;
using namespace Application;

void Instrument_calibration_provider::v_BIAS(Instruction_major &instruction) {
    printf("Bias: setpoint=%d, voltage==%d, error=%d\r\n",
           use_bias.get_C25_setpoint(), use_bias.get_voltage(), use_bias.get_error());
    instruction.print_ack();
}

void Instrument_calibration_provider::v_BIAS_ADC(Instruction_major &instruction) {
    if (uint32_t raw{}; use_bias.get_ADC().read_raw_ADC(raw)) {
        instruction.print_ack(static_cast<int32_t>(raw));
    } else {
        instruction.print_nack(123);
    }
}

void Instrument_calibration_provider::v_BIAS_ADC_SET(Instruction_major &instruction) {
    int32_t v30_2{};
    int32_t v40_3{};
    use_bias.get_ADC().get_setpoints(v30_2, v40_3);
    instruction.print_ack(v30_2, v40_3);
}

void Instrument_calibration_provider::v_BIAS_ADC_SET_v30_v40(Instruction_major &instruction, const int v30_2, const int v40_3) {
    use_bias.get_ADC().set_setpoints(v30_2, v40_3);
    if (use_repository.set(Cal_ADC_35V, v30_2).success() && use_repository.set(Cal_ADC_40V, v40_3).success()) {
        instruction.print_ack(v30_2, v40_3);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Instrument_calibration_provider::v_BIAS_C25(Instruction_major &instruction) {
    auto bias{use_bias.get_C25_setpoint()};
    instruction.print_ack(bias);
}

void Instrument_calibration_provider::v_BIAS_C25_bias_mV(Instruction_major &instruction, const int bias_mV_2) {
    use_bias.set_C25_setpoint(bias_mV_2);
    if (use_repository.set(Cal_bias_25C, bias_mV_2).success()) {
        instruction.print_ack(bias_mV_2);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Instrument_calibration_provider::v_BIAS_CAL(Instruction_major &instruction) {
    the_calibration_flag = !the_calibration_flag;
    printf("The bias calibration mode is %s\r\n", the_calibration_flag ? "ON" : "OFF");
    instruction.print_ack();
}

void Instrument_calibration_provider::v_BIAS_DIAG(Instruction_major &instruction) {
    use_bias.print_diag();
    instruction.print_ack();
}

void Instrument_calibration_provider::v_BIAS_ERROR(Instruction_major &) {
}

void Instrument_calibration_provider::v_BIAS_SET(Instruction_major &) {
}

void Instrument_calibration_provider::v_BIAS_SET_bias_mV(Instruction_major &, int bias_mV_2) {
}

void Instrument_calibration_provider::v_BIAS_TEMP(Instruction_major &) {
}

void Instrument_calibration_provider::v_BIAS_TEMP_SET(Instruction_major &) {
}

void Instrument_calibration_provider::v_BIAS_TEMP_SET_a_b_c(Instruction_major &, int a_3, int b_4, int c_5) {
}

void Instrument_calibration_provider::v_BIAS_VOLT(Instruction_major &instruction) {
    auto bias{use_bias.get_voltage()};
    instruction.print_ack(bias);
}

void Instrument_calibration_provider::v_BIAS_DAC(Instruction_major &instruction) {
    instruction.print_ack(static_cast<int32_t>(use_bias.get_DAC().get_raw_DAC()));
}

void Instrument_calibration_provider::v_BIAS_DAC_data12(Instruction_major &instruction, const int data12_2) {
    enum { DAC_min = 0, DAC_max = 4095 }; // 12-bit DAC; out-of-range values would wrap on hardware
    if (data12_2 < DAC_min || data12_2 > DAC_max) {
        instruction.print_nack(123);
    } else if (use_bias.get_DAC().set_raw_DAC(static_cast<uint32_t>(data12_2))) {
        instruction.print_ack(data12_2);
    } else {
        instruction.print_nack(123);
    }
}

void Instrument_calibration_provider::v_BIAS_DAC_SET(Instruction_major &instruction) {
    int32_t v30{};
    int32_t v40{};
    use_bias.get_DAC().get_setpoints(v30, v40);
    instruction.print_ack(v30, v40);
}

void Instrument_calibration_provider::v_BIAS_DAC_SET_v30_v40(Instruction_major &instruction, const int v30_3, const int v40_4) {
    use_bias.get_DAC().set_setpoints(v30_3, v40_4);
    if (use_repository.set(Cal_DAC_35V, v30_3).success() && use_repository.set(Cal_DAC_40V, v40_4).success()) {
        instruction.print_ack(v30_3, v40_4);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Instrument_calibration_provider::v_CAL_DUMP(Instruction_major &instruction) {
    the_test_pedestal.print_pedestals();
    instruction.print_ack();
}

void Instrument_calibration_provider::v_CAL_DIAG(Instruction_major &instruction) {
    the_test_pedestal.print_diag();
    the_test_noise_floor.print_diag();
    instruction.print_ack();
}

void Instrument_calibration_provider::v_CAL_TEST_BIAS(Instruction_major &instruction) {
    if (int32_t value; use_repository.get(Active_mode, value).success() && Mode_idle == value) {
        instruction.print_nack("NOT IMPLEMENTED");
    } else {
        instruction.print_nack("NOT IN IDLE MODE");
    }
}

void Instrument_calibration_provider::v_CAL_TEST_OFFSET(Instruction_major &instruction) {
    if (int32_t value; use_repository.get(Active_mode, value).success() && Mode_idle == value) {
        instruction.print_nack("NOT IMPLEMENTED");
    } else {
        instruction.print_nack("NOT IN IDLE MODE");
    }
}

void Instrument_calibration_provider::v_CAL_TEST_NOISE(Instruction_major &instruction) {
    if (int32_t value; use_repository.get(Active_mode, value).success() && Mode_idle == value) {
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
    } else {
        instruction.print_nack("NOT IN IDLE MODE");
    }
}

void Instrument_calibration_provider::v_CAL_TEST_PEDESTAL(Instruction_major &instruction) {
    if (int32_t value; use_repository.get(Active_mode, value).success() && Mode_idle == value) {
        the_test_pedestal.set_trace(the_trace_flag);
        if (the_test_pedestal.start_test(&instruction).success()) {
            instruction.print_pending();
        } else {
            instruction.print_nack(the_test_pedestal.get_diag_code());
        }
    } else {
        instruction.print_nack("NOT IN IDLE MODE");
    }
}

void Instrument_calibration_provider::v_CAL_TRACE(Instruction_major &) {
    the_trace_flag = !the_trace_flag;
    printf("The calibration trace is %s\r\n", the_trace_flag ? "ON" : "OFF");
}
