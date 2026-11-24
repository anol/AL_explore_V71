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
import :Sensor_TMP1075;
import Type.Abstract_I2C;
import Type.Abstract_board;
import Platform.FreeRTOS_task;

namespace Temperature_sensor {
    export class Temperature_task : public FreeRTOS::FreeRTOS_task {
        enum {
            Priority          = Target_config::Temperature_task_priority,
            Stack_size        = Target_config::Temperature_task_stack_size,
            Number_of_sensors = 6,
        };

        Abstract::Abstract_I2C &use_I2C;
        Abstract::Abstract_semaphore *optional_start_semaphore{};
        int32_t the_temperature{2000};
        Sensor_TMP1075 the_sensor_table[Number_of_sensors]{
            {"U07", 0b0100'1000},
            {"U13", 0b0100'1001},
            {"U14", 0b0100'1010},
            {"U15", 0b0100'1011},
            {"Uxx", 0b0100'1100},
            {"Uyy", 0b0100'1101}
        };

    public:
        Temperature_task(Abstract::Abstract_board &board)
            : FreeRTOS_task("Temperature task", Priority, Stack_size),
              use_I2C(board.get_I2C()) {
        }

        void initialize() override {
        };

        void task_loop() override;

        void attach_start_semaphore(Abstract::Abstract_semaphore &semaphore) { optional_start_semaphore = &semaphore; }

        [[nodiscard]] int32_t get_temperature() const { return the_temperature; }

        void override_temperature(int32_t temperature) { the_temperature = temperature; }

        void print_temperatures() const;

    private:
        void read_all_temperatures();
    };
}
