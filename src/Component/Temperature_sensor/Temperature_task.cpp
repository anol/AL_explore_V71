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

namespace Temperature_sensor {
    void Temperature_task::initialize() {
    }

    void Temperature_task::task_loop() {
        enum { Delay_millis = 2000 };
        while (true) {
            delay_until(Delay_millis);
            read_all_temperatures();
        }
    }

    void Temperature_task::print_temperatures() const {
        printf("Temperature: %d.%d\r\n", the_temperature / 10, the_temperature % 10);
        for (auto &sensor: the_sensor_table) {
            printf("  %s: addr=%d, temp=%d\r\n", sensor.get_name(), sensor.get_address(), sensor.get_temperature());
        }
    }

    void Temperature_task::read_all_temperatures() {
        int32_t accumelator{};
        int32_t counter{};
        for (auto &sensor: the_sensor_table) {
            if (sensor.read(use_I2C)) {
                accumelator += sensor.get_temperature();
                counter++;
            }
        }
        the_temperature = accumelator / counter;
    }
}
