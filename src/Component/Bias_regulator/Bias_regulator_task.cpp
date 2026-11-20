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
* @file   Bias_regulator_task.cpp
* @author AndersEmilOlsen, IDEAS
* @date   19.11.2026
* @brief  
*/

module Component.Bias_regulator;

namespace Bias_regulator {
    void Bias_regulator_task::initialize() {
    }

    void Bias_regulator_task::task_loop() {
        enum { Delay_millis = 1000 };
        while (true) {
            if (optional_start_semaphore) {
                optional_start_semaphore->take(); // blocks during startup and calibration mode
                restart_period();                 // no catch-up burst after a long halt
                the_calibration.bias_temperature_control();
                optional_start_semaphore->give();
            }
            delay_until(Delay_millis);
        }
    }
}
