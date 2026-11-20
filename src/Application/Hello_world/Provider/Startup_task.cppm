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
* @file   Startup_task.cppm
* @author AndersEmilOlsen, IDEAS
* @date   25.09.2026
* @brief  
*/

module;
#include "Target_config.h"

export module Application.Startup_task;
import Platform.FreeRTOS_task;
import Support.Configuration_repository;
import Component.Bias_calibration;
import Component.IDE3380_provider;

namespace Application
{
    /**
     * Runs once at startup: loads the configuration and applies it to the ASIC registers and the bias
     * setpoints, then reports back to its owner (the mode controller), which leaves startup mode.
     */
    export class Startup_task : public FreeRTOS::FreeRTOS_task
    {
    public:
        using Done_func = void (*)(void* user);

    private:
        enum
        {
            Priority = Target_config::Startup_task_priority,
            Stack_size = Target_config::Startup_task_stack_size
        };

        IDE3380::IDE3380_provider& use_IDE3380;
        Calibration::Bias_calibration* optional_bias{}; // attached by the mode controller
        Repository::Configuration_repository& use_repository;
        void* use_done_user;
        Done_func use_done_func;

    public:
        Startup_task(
            IDE3380::IDE3380_provider& IDE3380,
            Repository::Configuration_repository& repository,
            void* done_user, const Done_func done_func
        )
            : FreeRTOS_task("Startup task", Priority, Stack_size),
              use_IDE3380(IDE3380),
              use_repository(repository),
              use_done_user(done_user),
              use_done_func(done_func)
        {
        };

        void attach_bias(Calibration::Bias_calibration& bias) { optional_bias = &bias; }

        void initialize() override;

        void task_loop() override;
    };
}
