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
* @file   Mode_control_provider.cpp
* @author AndersEmilOlsen, IDEAS
* @date   21.05.2026
* @brief  
*/

module;
#include <cstdint>
#include "Persistent_parameter_id.h"
#include <cstdio>

module Application.Mode_control_provider;
import Component.Histogram_storage;
import Component.IDE3380_interface;
import Component.Bias_calibration;
import Domain.SpectraNode_keyword_lookup;
import Domain.SpectraNode_command_lookup;
import Domain.SpectraNode_provider_indication;
import Support.Configuration_repository;
import Support.Instruction_major;
import Application.Cadence_control;
import Application.Spectroscopic_data_provider;

using namespace IDE3380;

using namespace SpectraNode_interface;
using namespace Application;

const char *Mode_control_provider::Operation_mode_to_text(const Operation_mode mode) {
    switch (mode) {
        case Mode_idle: return "IDLE";
        case Mode_nominal: return "NOMINAL";
        case Mode_demo: return "DEMO";
        default: return "?";
    }
}

void Mode_control_provider::print_status() const {
    printf(" mode=%s, cadence=%d", Operation_mode_to_text(the_mode), use_cadence.get_cadence());
}

Status_code Mode_control_provider::set_mode(const Operation_mode mode, const Parameter_id channel,
                                     const Parameter_id format, const Parameter_id cadence) {
    set_operation_mode(mode);
    auto success{set_parameter(Active_mode, mode, true).success()};
    int32_t value;
    success = get_parameter(channel, value, success).success();
    use_data_provider.set_channel(value);
    success = get_parameter(format, value, success).success();
    use_data_provider.set_format(static_cast<Data_format>(value));
    success = get_parameter(cadence, value, success).success();
    use_cadence.set_cadence(value);
    use_cadence.set_next_wakeup();
    return Status_code(success);
}

void Mode_control_provider::v_MODE_DEMO(Instruction_major &instruction) {
    if (set_mode(Mode_demo, Channel_demo, Format_demo, Cadence_demo).success()) {
        instruction.print_ack();
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Mode_control_provider::v_MODE_IDLE(Instruction_major &instruction) {
    if (set_mode(Mode_idle, Channel_idle, Format_idle, Cadence_idle).success()) {
        instruction.print_ack();
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Mode_control_provider::v_MODE_NOMINAL(Instruction_major &instruction) {
    if (set_mode(Mode_nominal, Channel_nominal, Format_nominal, Cadence_nominal).success()) {
        instruction.print_ack();
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Mode_control_provider::v_IDLE_CADENCE_seconds(Instruction_major &instruction, int seconds_2) {
    use_cadence.set_cadence(seconds_2);
    if (use_repository.set(Cadence_idle, seconds_2).success()) {
        instruction.print_ack(seconds_2);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Mode_control_provider::v_DEMO_CADENCE_seconds(Instruction_major &instruction, int seconds_2) {
    use_cadence.set_cadence(seconds_2);
    if (use_repository.set(Cadence_demo, seconds_2).success()) {
        instruction.print_ack(seconds_2);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Mode_control_provider::v_DEMO_CHANNEL_channel(Instruction_major &instruction, int channel_2) {
    use_data_provider.set_channel(channel_2);
    if (use_repository.set(Channel_demo, channel_2).success()) {
        instruction.print_ack(channel_2);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Mode_control_provider::v_NOMINAL_ALARM_micro_sievert(Instruction_major &, int micro_sievert_2) {
    printf("%s %s=%d\r\n", "Mode_control_provider::v_NOMINAL_ALARM_micro_sievert:",
           "micro_sievert_2=", micro_sievert_2);
}

void Mode_control_provider::v_NOMINAL_CADENCE_seconds(Instruction_major &instruction, int seconds_2) {
    use_cadence.set_cadence(seconds_2);
    if (use_repository.set(Cadence_nominal, seconds_2).success()) {
        instruction.print_ack(seconds_2);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}
