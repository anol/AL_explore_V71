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
import Component.Temperature_sensor;

namespace Bias_regulator {
    export class Bias_regulator_task : public FreeRTOS::FreeRTOS_task {
        enum {
            Priority   = Target_config::Bias_regulator_task_priority,
            Stack_size = Target_config::Bias_regulator_task_stack_size,
        };

        Abstract::Abstract_I2C &use_I2C;
        Temperature_sensor::Temperature_task &use_temperature_sensor;
        Calibration::Bias_calibration the_calibration;
        Abstract::Abstract_semaphore *optional_start_semaphore{};

    public:
        explicit Bias_regulator_task(Abstract::Abstract_board &board, Temperature_sensor::Temperature_task &temperature_sensor)
            : FreeRTOS_task("Bias regulator", Priority, Stack_size),
              use_I2C(board.get_I2C()),
              use_temperature_sensor(temperature_sensor),
              the_calibration(board) {
        }

        void initialize() override {
            the_calibration.initialize();
        }

        void attach_start_semaphore(Abstract::Abstract_semaphore &semaphore) {
            optional_start_semaphore = &semaphore;
        }

        Calibration::Bias_calibration &get_calibration() {
            return the_calibration;
        }

        void task_loop() override {
            enum { Start_delay_millis = 300, Periodic_delay_millis = 1000 };
            delay_until(Start_delay_millis);
            while (true) {
                delay_until(Periodic_delay_millis);
                auto temperature = use_temperature_sensor.get_temperature();
                if (optional_start_semaphore) {
                    optional_start_semaphore->take(); // blocks during startup and calibration mode
                    restart_period();                 // no catch-up burst after a long halt
                    the_calibration.bias_temperature_control(temperature);
                    optional_start_semaphore->give();
                } else {
                    the_calibration.bias_temperature_control(temperature);
                }
            }
        }
    };
}
