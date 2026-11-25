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
* @file   Temperature_sensor.cpp
* @author AndersEmilOlsen, IDEAS
* @date   19.11.2026
* @brief  
*/

module;

#include <cstdint>
#include <cstdio>

module Component.Temperature_sensor;
import Utility.Unit_converter;

namespace Temperature_sensor {
    void Temperature_task::task_loop() {
        enum { Start_delay_millis = 400, Periodic_delay_millis = 2000 };
        delay_until(Start_delay_millis);
        while (true) {
            delay_until(Periodic_delay_millis);
            if (optional_start_semaphore) {
                optional_start_semaphore->take(); // blocks during startup and calibration mode
                restart_period();                 // no catch-up burst after a long halt
                read_all_temperatures();
                optional_start_semaphore->give();
            } else {
                read_all_temperatures();
            }
        }
    }

    void Temperature_task::print_temperatures() const {
        const auto average = Unit_converter::centidegrees_to_parts(the_temperature);
        printf("  Temp: %s%d.%02d\r\n", average.sign, average.degrees, average.hundredths);
        for (auto &sensor: the_sensor_table) {
            if (sensor.is_good()) {
                const auto [sign, degrees, hundredths] = Unit_converter::centidegrees_to_parts(sensor.get_temperature());
                printf("  %s: addr=%d, temp=%s%d.%02d\r\n", sensor.get_name(), sensor.get_address(),
                       sign, degrees, hundredths);
            } else {
                printf("  %s: addr=%d <> NO DATA <>\r\n", sensor.get_name(), sensor.get_address());
            }
        }
    }

    void Temperature_task::read_all_temperatures() {
        int32_t accum{};
        int32_t counter{};
        for (auto &sensor: the_sensor_table) {
            if (sensor.read(use_I2C)) {
                accum += sensor.get_temperature();
                counter++;
            }
        }
        the_temperature = accum / counter;
    }
}
