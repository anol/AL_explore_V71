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

export module Component.Temperature_sensor;
import Type.Abstract_I2C;
import Type.Abstract_board;
import Platform.FreeRTOS_task;

namespace Temperature_sensor {
    export class Temperature_task : public FreeRTOS::FreeRTOS_task {
        enum {
            Priority   = Target_config::Temperature_task_priority,
            Stack_size = Target_config::Temperature_task_stack_size,
        };

        Abstract::Abstract_I2C &use_I2C;
        int32_t the_temperature{200};

    public:
        Temperature_task(Abstract::Abstract_board &board)
            : FreeRTOS_task("Temperature task", Priority, Stack_size),
              use_I2C(board.get_I2C()) {
        }

        void initialize() override;

        void task_loop() override;

        [[nodiscard]] int32_t get_temperature() const { return the_temperature; }
    };

}
