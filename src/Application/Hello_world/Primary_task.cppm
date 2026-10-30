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
import Support.Configuration_repository;
import Application.Detector;
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

        Detector& use_detector;
        Repository::Configuration_repository& use_repository;
        Mode_control_provider& use_mode_control;

    public:
        Primary_task(
            Detector& detector,
            Repository::Configuration_repository& repository,
            Mode_control_provider& mode_control
        )
            : FreeRTOS_task("Primary task", Priority, Stack_size),
              use_detector(detector),
              use_repository(repository),
              use_mode_control(mode_control)
        {
        };

        void initialize() override;

        void task_loop() override;
    };
}
