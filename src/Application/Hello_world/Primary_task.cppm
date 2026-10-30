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
* @file   Primary_task.cppm
* @author AndersEmilOlsen, IDEAS
* @date   25.09.2026
* @brief  
*/

module;

#include "Target_config.h"

export module Application.Primary_task;
import Platform.FreeRTOS_task;
import Component.IDE3380_interface;
import Component.Bias_calibration;
import Component.Histogram_storage;
import Support.Configuration_repository;
import Application.Command_handler;
import Application.Mode_control_provider;

namespace Application
{
    export class Primary_task : public FreeRTOS::FreeRTOS_task
    {
        enum
        {
            Priority = Target_config::Primary_task_priority,
            Stack_size = Target_config::Primary_task_stack_size
        };

        IDE3380::IDE3380_interface& use_IDE3380;
        Calibration::Bias_calibration& use_bias;
        Repository::Configuration_repository& use_repository;
        Command_handler the_command_handler;

    public:
        Primary_task(
            IDE3380::IDE3380_interface& IDE3380,
            Calibration::Bias_calibration& bias,
            Repository::Configuration_repository& repository,
            Histogram_storage& histogram,
            Mode_control_provider& mode_control
        )
            : FreeRTOS_task("Primary task", Priority, Stack_size),
              use_IDE3380(IDE3380),
              use_bias(bias),
              use_repository(repository),
              the_command_handler(histogram, IDE3380, mode_control, repository)
        {
        };

        void initialize() override;

        void task_loop() override;
    };
}
