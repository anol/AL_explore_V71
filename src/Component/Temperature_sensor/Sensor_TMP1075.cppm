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
* @file   Sensor_TMP1075.cppm
* @author AndersEmilOlsen, IDEAS
* @date   19.11.2026
* @brief  
*/

module;

#include <cstdint>
#include <cstdio>

module Component.Temperature_sensor:Sensor_TMP1075;
import Type.Abstract_I2C;

namespace Temperature_sensor
{
    class Sensor_TMP1075
    {
        const char* the_name;
        const uint8_t the_address;
        bool the_data_good_flag{};
        int32_t the_temperature{2000};

    public:
        Sensor_TMP1075(const char* name, uint8_t address)
            : the_name(name), the_address(address)
        {
        }

        bool read(Abstract::Abstract_I2C&);

        [[nodiscard]] int32_t get_temperature() const { return the_temperature; }

        [[nodiscard]] uint8_t get_address() const { return the_address; }

        [[nodiscard]] const char* get_name() const { return the_name; }

        [[nodiscard]] bool is_good() const { return the_data_good_flag; }
    };

    bool Sensor_TMP1075::read(Abstract::Abstract_I2C& I2C)
    {
        uint32_t data{};
        the_data_good_flag = I2C.read(the_address, 2, data);
        if (the_data_good_flag)
        {
            const auto temp = static_cast<int32_t>(data);
            const int32_t degrees = 100 * ((temp >> 8) & 0xFF);
            const int32_t fraction = 100 * ((temp >> 4) & 0xF) / 16;
            the_temperature = degrees + fraction;
            printf("%d.%d ", the_temperature / 100, the_temperature % 100);
        }
        return the_data_good_flag;
    }
}
