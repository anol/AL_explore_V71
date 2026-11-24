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
* @file   Temperature_sensor.cppm
* @author AndersEmilOlsen, IDEAS
* @date   19.11.2026
* @brief  
*/
module;

#include <cstdint>
#include "Target_config.h"

export module Component.Bias_regulator;
import Type.Abstract_I2C;
import Type.Abstract_board;
import Platform.FreeRTOS_task;
import Type.Abstract_semaphore;
import Component.Bias_calibration;

namespace Bias_regulator {
    export class Bias_regulator_task : public FreeRTOS::FreeRTOS_task {
        enum {
            Priority   = Target_config::Bias_regulator_task_priority,
            Stack_size = Target_config::Bias_regulator_task_stack_size,
        };

        Abstract::Abstract_I2C &use_I2C;
        Calibration::Bias_calibration the_calibration;
        Abstract::Abstract_semaphore *optional_start_semaphore{};
        int32_t the_temperature{200};

    public:
        explicit Bias_regulator_task(Abstract::Abstract_board &board)
            : FreeRTOS_task("Bias regulator", Priority, Stack_size),
              use_I2C(board.get_I2C()),
              the_calibration(board) {
        }

        void initialize() override;

        void task_loop() override;

        void attach_start_semaphore(Abstract::Abstract_semaphore &semaphore) { optional_start_semaphore = &semaphore; }

        [[nodiscard]] int32_t get_temperature() const { return the_temperature; }

        Calibration::Bias_calibration &get_calibration() { return the_calibration; }
    };

}
