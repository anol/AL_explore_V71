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


#include "Instrument_calibration_provider.h"

#include <cstdio>
#include "Bias_calibration.h"
import Support.Configuration_repository;

void Instrument_calibration_provider::v_CAL_ADC_V35_cal_35V(Instruction_major &instruction, const int cal_35V_3) {
    use_bias.get_ADC().set_cal_35V(cal_35V_3);
    if (use_repository.set(Application::Cal_ADC_35V, cal_35V_3).success()) {
        instruction.print_ack(cal_35V_3);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Instrument_calibration_provider::v_CAL_ADC_V40_cal_40V(Instruction_major &instruction, const int cal_40V_3) {
    use_bias.get_ADC().set_cal_45V(cal_40V_3);
    if (use_repository.set(Application::Cal_ADC_40V, cal_40V_3).success()) {
        instruction.print_ack(cal_40V_3);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Instrument_calibration_provider::v_CAL_DAC_V35_cal_35V(Instruction_major &instruction, int cal_35V_3) {
    use_bias.get_DAC().set_cal_35V(cal_35V_3);
    if (use_repository.set(Application::Cal_DAC_35V, cal_35V_3).success()) {
        instruction.print_ack(cal_35V_3);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Instrument_calibration_provider::v_CAL_DAC_V40_cal_40V(Instruction_major &instruction, int cal_40V_3) {
    use_bias.get_DAC().set_cal_45V(cal_40V_3);
    if (use_repository.set(Application::Cal_DAC_40V, cal_40V_3).success()) {
        instruction.print_ack(cal_40V_3);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Instrument_calibration_provider::v_CAL_TEST_BIAS(Instruction_major &instruction) {
    if (int32_t value; use_repository.get(Application::Active_mode, value).success() && Application::Mode_idle == value) {
        instruction.print_nack("NOT IMPLEMENTED");
    } else {
        instruction.print_nack("NOT IN IDLE MODE");
    }
}

void Instrument_calibration_provider::v_CAL_TEST_OFFSET(Instruction_major &instruction) {
    if (int32_t value; use_repository.get(Application::Active_mode, value).success() && Application::Mode_idle == value) {
        instruction.print_nack("NOT IMPLEMENTED");
    } else {
        instruction.print_nack("NOT IN IDLE MODE");
    }
}

void Instrument_calibration_provider::v_CAL_TEST_NOISE(Instruction_major &instruction) {
    if (int32_t value; use_repository.get(Application::Active_mode, value).success() && Application::Mode_idle == value) {
        int32_t integration_time{};
        int32_t start_threshold{};
        int32_t stop_count{};
        if (use_repository.get(Application::Cal_integration_time, integration_time).success() &&
            use_repository.get(Application::Cal_start_threshold, start_threshold).success() &&
            use_repository.get(Application::Cal_stop_count, stop_count).success()) {
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
    if (int32_t value; use_repository.get(Application::Active_mode, value).success() && Application::Mode_idle == value) {
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

void Instrument_calibration_provider::v_CAL_DIAG(Instruction_major &instruction) {
    the_test_pedestal.print_diag();
    the_test_noise_floor.print_diag();
    instruction.print_ack();
}

void Instrument_calibration_provider::v_CAL_TRACE(Instruction_major &) {
    the_trace_flag = !the_trace_flag;
    printf("The calibration trace is %s\r\n", the_trace_flag ? "ON" : "OFF");
}
