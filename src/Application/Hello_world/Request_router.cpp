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
#include "Persistent_parameter_id.h"
#include <cstdio>

module Application.Request_router;
import Component.Histogram_storage;
import Component.IDE3380_interface;
import Component.Bias_calibration;
import Component.Event_counter;
import Type.Abstract_provider;
import Application.Cadence_control;
import Application.Configuration_manager_provider;
import Application.Housekeeping_provider;
import Application.Instrument_calibration_provider;
import Application.Mode_control_provider;
import Application.Spectroscopic_data_provider;
import Support.Configuration_repository;
import Support.Instruction_major;
import Domain.SpectraNode_keyword_lookup;
import Domain.SpectraNode_command_lookup;
import Domain.SpectraNode_provider_indication;

using namespace IDE3380;

using namespace SpectraNode_interface;
using namespace Instruction;

namespace Application
{
    void Request_router::initialize()
    {
        use_IDE3380.initialize(
            this,
            [](void* user)
            {
                if (user)
                {
                    static_cast<Request_router*>(user)->wakeup_from_sleep();
                }
            },
            [](void* user, const TXD_data_t& data)
            {
                if (user)
                {
                    static_cast<Request_router*>(user)->update_histogram(data);
                }
            });
    }

    bool Request_router::on_indication(Instruction_major& instruction)
    {
        if (Housekeeping_provider::is_trace())
        {
            instruction.print_trace(SpectraNode_interface::get_keyword);
        }
        bool executed = use_mode_control.on_indication(instruction);
        if (!executed) { executed = use_data_provider.on_indication(instruction); }
        if (!executed) { executed = the_instrument_calibration.on_indication(instruction); }
        if (!executed) { executed = the_configuration_manager.on_indication(instruction); }
        if (!executed) { executed = use_housekeeper.on_indication(instruction); }
        if (!executed)
        {
            printf("ERROR: NO_SUCH COMMAND, DIAG=%d.\r\n", (int)instruction.get_command_id());
        }
        return executed;
    }

    Status_code Request_router::update_mode()
    {
        int32_t value;
        auto success{use_repository.get(Active_mode, value)};
        auto mode = static_cast<Operation_mode>(value);
        switch (mode)
        {
        default:
            mode = Mode_idle;
        case Mode_idle:
            use_mode_control.set_mode(mode, Channel_idle, Format_idle, Cadence_idle);
            break;
        case Mode_nominal:
            use_mode_control.set_mode(mode, Channel_nominal, Format_nominal, Cadence_nominal);
            break;
        case Mode_demo:
            use_mode_control.set_mode(mode, Channel_demo, Format_demo, Cadence_demo);
            break;
        }
        return success;
    }
} // Application
