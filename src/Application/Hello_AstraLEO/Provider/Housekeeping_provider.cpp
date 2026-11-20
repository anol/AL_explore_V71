/*
* Copyright (C) 2026 Integrated Detector Electronics AS
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
* @file   Housekeeping_provider.cpp
* @author AndersEmilOlsen, IDEAS
* @date   21.05.2026
* @brief  
*/

module;
#include <cstdint>
#include <cstdio>
#include "Target_config.h"

module Application.Housekeeping_provider;
import Component.Histogram_storage;
import Component.IDE3380_provider;
import Component.Bias_calibration;
import Domain.Keyword_lookup;
import Application.AstraLEO_command_lookup;
import Application.AstraLEO_provider_indication;
import Support.Configuration_repository;
import Support.Instruction_major;
import Support.CLI_help;
import Application.Cadence_control;

using namespace IDE3380;

using namespace AstraLEO_interface;
using namespace Application;
using namespace Instruction;

using namespace Instruction;

bool Housekeeping_provider::the_trace_flag{false};
/// @brief Show miscellaneous diagnostic information.
void Housekeeping_provider::v_DIAG(Instruction_major &instruction) {
    printf("Diagnostic info\r\n");
    use_histogram.print_diag();
    the_bias_regulator_task.get_calibration().print_diag();
    use_IDE3380.print_diag();
    use_repository.print_diag();
    use_board.print_diagnostics();
    instruction.print_ack();
}

/// @brief Toggle console echo ON/OFF.
void Housekeeping_provider::v_ECHO(Instruction_major &instruction) {
    const auto echo = use_console.toggle_echo();
    printf("The console echo is %s\r\n", echo ? "ON" : "OFF");
    instruction.print_ack();
}

/// @brief Show the list of commands.
void Housekeeping_provider::v_HELP(Instruction_major &instruction) {
    const auto is_AT_mode = instruction.is_AT_mode();
    const auto is_question = Question_mark == instruction.get_token_type(0);
    const auto is_sub_question = Question_mark == instruction.get_token_type(1);
    const auto command = instruction.get_token_id(0);
    const auto success = use_console.print_help(command, is_question, is_sub_question, is_AT_mode);
    if (success) {
        instruction.print_ack();
    } else {
        instruction.print_nack(command);
    }
}

void Housekeeping_provider::print_housekeeping() {
    printf("Housekeeping\r\n");
    the_temperature_task.print_temperatures();
}

/// @brief Show the housekeeping report.
void Housekeeping_provider::v_HOUSE(Instruction_major &instruction) {
    print_housekeeping();
    instruction.print_ack();
}

/// @brief Show the system status report.
void Housekeeping_provider::v_STATUS(Instruction_major &instruction) {
    printf("Status:");
    use_status_func(use_status_user);
    printf("\r\n");
    instruction.print_ack();
}

/// @brief Initiate a self-test sequence.
void Housekeeping_provider::v_TEST(Instruction_major &instruction) {
    printf("<> Sorry, not implemented. <>\r\n");
    instruction.print_nack(1);
}

/// @brief Toggle the command trace ON/OFF.
void Housekeeping_provider::v_TRACE(Instruction_major &) {
    the_trace_flag = !the_trace_flag;
    IDE3380::IDE3380_service_task::set_trace(the_trace_flag);
    printf("The command trace is %s\r\n", the_trace_flag ? "ON" : "OFF");
}

void Housekeeping_provider::print_version() {
    printf("----------------------------\r\n");
    printf("-- " IDEAS_PRODUCT_ID " " BUILD_INFORMATION "\r\n");
    printf("-- " GIT_REPO "\r\n");
    printf("-- " ARCH_NAME " " __VERSION__ "\r\n");
    printf("-- " __DATE__ " " __TIME__ "\r\n");
    printf("----------------------------\r\n");
    // int32_t serial{};
    // if (use_repository.get(Serial_number, serial).success())
    // {
    //     printf("S/N xxxx-x-%d.\r\n", (int)serial);
    // }
}

/// @brief Show the version and build report.
void Housekeeping_provider::v_VERSION(Instruction_major &instruction) {
    print_version();
    instruction.print_ack();
}
