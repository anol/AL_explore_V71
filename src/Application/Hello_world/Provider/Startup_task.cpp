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
* @file   Startup_task.cpp
* @author AndersEmilOlsen, IDEAS
* @date   25.09.2026
* @brief  
*/

module;

#include <cstdio>

#include "Persistent_parameter_id.h"

module Application.Startup_task;
import Platform.FreeRTOS_task;
import Application.Housekeeping_provider;
import Component.IDE3380_provider;

namespace Application {
    void Startup_task::initialize() {
        use_IDE3380.attach_repository(use_repository);
    }

    void Startup_task::task_loop() {
        enum { Start_delay_millis = 100, Delay_millis = 1000 };
        delay_until(Start_delay_millis);
        Housekeeping_provider::print_version();
        use_repository.initialize();
        auto success = use_repository.load();
        success = use_IDE3380.update_registers(IDE3380_0);
        if (success.failed()) {
            printf("<> Failed to update IDE3380 registers <>\r\n");
        }
        success = optional_bias ? optional_bias->update_setpoints(use_repository) : Status_code::Failure();
        if (success.failed()) {
            printf("<> Failed to update bias setpoints <>\r\n");
        }
        if (use_done_func) {
            use_done_func(use_done_user); // the mode controller leaves startup mode
        }
        while (true) {
            delay_until(Delay_millis);
        }
    }
}
