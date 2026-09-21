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
#include "Persistent_parameter_id.h"

module Application.Housekeeping_provider;
import Component.Histogram_storage;
import Component.IDE3380_interface;
import Component.Bias_calibration;
import Domain.SpectraNode_keyword_lookup;
import Domain.SpectraNode_command_lookup;
import Domain.SpectraNode_provider_indication;
import Support.Configuration_repository;
import Support.Instruction_major;
import Support.CLI_help;
import Application.Cadence_control;
import Application.Mode_control_provider;
import Application.Spectroscopic_data_provider;

using namespace IDE3380;

using namespace SpectraNode_interface;
using namespace Application;
using namespace Instruction;

// #include "stm32u5xx_hal.h"

using namespace Instruction;

bool Housekeeping_provider::the_trace_flag{true};

void Housekeeping_provider::print_version()
{
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

void Housekeeping_provider::v_VERSION(Instruction_major& instruction)
{
    print_version();
    instruction.print_ack();
}

void Housekeeping_provider::v_STATUS(Instruction_major& instruction)
{
    printf("Status:");
    use_mode_control.print_status();
    use_data_provider.print_status();
    use_bias.print_status();
    // printf(", tick=%d\r\n", HAL_GetTick());
    instruction.print_ack();
}

void Housekeeping_provider::v_DIAG(Instruction_major& instruction)
{
    printf("Diagnostic info\r\n");
    use_histogram.print_diag();
    use_bias.print_diag();
    use_IDE3380.print_diag();
    use_repository.print_diag();
    instruction.print_ack();
}

void Housekeeping_provider::v_TEST(Instruction_major&)
{
    printf("Test_utility_provider::v_TEST:\r\n");
}

void Housekeeping_provider::v_HELP(Instruction_major& instruction)
{
    the_help.set_mode(instruction.is_AT_mode());
    if (the_help.is_help_command(instruction.get_token_id(0)) ||
        instruction.get_token_type(0) == Question_mark)
    {
        the_help.print(instruction.get_token_type(1) == Question_mark,
                       number_of_keys);
    }
    else if (instruction.get_token_type(1) == Question_mark)
    {
        the_help.print((uint32_t)instruction.get_token_id(0));
    }
    else
    {
        the_help.print(false, number_of_keys);
    }
    instruction.print_ack();
}

void Housekeeping_provider::v_TRACE(Instruction_major&)
{
    the_trace_flag = !the_trace_flag;
    printf("The command trace is %s\r\n", the_trace_flag ? "ON" : "OFF");
}
