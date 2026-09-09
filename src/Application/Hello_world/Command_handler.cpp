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


#include "Command_handler.h"
#include "Histogram_storage.h"
#include <stdio.h>
#include "IDE3380_interface.h"


namespace Application {
    void Command_handler::initialize() {
        use_IDE3380.initialize(
            this,
            [](void *user) {
                if (user) {
                    static_cast<Command_handler *>(user)->wakeup_from_sleep();
                }
            },
            [](void *user, const TXD_data_t &data) {
                if (user) {
                    static_cast<Command_handler *>(user)->update_histogram(data);
                }
            });
    }

    void Command_handler::on_indication(Instruction_major &instruction) {
        bool executed = the_mode_control.on_indication(instruction);
        if (!executed) { executed = the_data_provider.on_indication(instruction); }
        if (!executed) { executed = the_instrument_calibration.on_indication(instruction); }
        if (!executed) { executed = the_configuration_manager.on_indication(instruction); }
        if (!executed) { executed = the_housekeeper.on_indication(instruction); }
        if (!executed) {
            printf("ERROR: NO_SUCH COMMAND, DIAG=%d.\r\n", (int)instruction.get_command_id());
        }
    }

    bool Command_handler::update_mode() {
        int32_t value;
        auto success{use_repository.get(Active_mode, value)};
        auto mode = static_cast<Operation_mode>(value);
        switch (mode) {
            default:
                mode = Mode_idle;
            case Mode_idle:
                the_mode_control.set_mode(mode, Channel_idle, Format_idle, Cadence_idle);
                break;
            case Mode_nominal:
                the_mode_control.set_mode(mode, Channel_nominal, Format_nominal, Cadence_nominal);
                break;
            case Mode_demo:
                the_mode_control.set_mode(mode, Channel_demo, Format_demo, Cadence_demo);
                break;
        }
        return success;
    }
} // Application
