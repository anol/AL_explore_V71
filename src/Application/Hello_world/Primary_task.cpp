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
* @file   Primary_task.cpp
* @author AndersEmilOlsen, IDEAS
* @date   25.09.2026
* @brief  
*/

module;

#include <cstdio>

#include "Persistent_parameter_id.h"

module Application.Primary_task;
import Platform.FreeRTOS_task;
import Application.Housekeeping_provider;
import Component.IDE3380_interface;

namespace Application {
    void Primary_task::initialize() {
        use_bias.initialize();
        use_command_handler.initialize();
    }

    void Primary_task::task_loop() {
        Housekeeping_provider::print_version();
        use_repository.initialize();
        auto success = use_repository.load();
        use_command_handler.update_mode();
        if (success.success()) {
            use_IDE3380.update_registers(use_repository, IDE3380_0);
        } else {
            printf("<> Failed to load repository <>\r\n");
        }
        success = use_bias.update_setpoints(use_repository);
        if (success.failed()) {
            printf("<> Failed to update bias setpoints <>\r\n");
        }
        enum { Delay_millis = 999 };
        while (true) {
            delay_until(Delay_millis);
        }
    }
}
