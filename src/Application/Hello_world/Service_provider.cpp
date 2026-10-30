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
* @file   Command_handler.cpp
* @author AndersEmilOlsen, IDEAS
* @date   05.02.2026
* @brief  
*/

module;
#include <cstdint>
#include <cstdio>

module Application.Service_provider;
import Application.Mode_control_provider;
import Application.Cadence_control;
import Application.Configuration_manager_provider;
import Application.Housekeeping_provider;
import Application.Instrument_calibration_provider;
import Application.Spectroscopic_data_provider;
import Domain.SpectraNode_keyword_lookup;
import Domain.SpectraNode_command_lookup;
import Domain.SpectraNode_provider_indication;

using namespace SpectraNode_interface;
using namespace Instruction;

namespace Application
{

    bool Service_provider::on_indication(Instruction_major& instruction)
    {
        if (Housekeeping_provider::is_trace())
        {
            instruction.print_trace(get_keyword);
        }
        bool executed = the_mode_control.on_indication(instruction);
        if (!executed) { executed = the_data_provider.on_indication(instruction); }
        if (!executed) { executed = the_instrument_calibration.on_indication(instruction); }
        if (!executed) { executed = the_configuration_manager.on_indication(instruction); }
        if (!executed) { executed = the_housekeeper.on_indication(instruction); }
        if (!executed)
        {
            printf("ERROR: NO_SUCH COMMAND, DIAG=%d.\r\n", static_cast<int>(instruction.get_command_id()));
        }
        return executed;
    }

} // Application
