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
* @file   IDE3380_register_access.h
* @author AndersEmilOlsen, IDEAS
* @date   13.05.2026
* @brief
*/


#pragma once

#include <cstdint>
#include "IDE3380_definitions.h"

namespace Abstract {
    class Abstract_SPI;
}

namespace IDE3380 {
    class IDE3380_register_access {
        Abstract::Abstract_SPI &use_SPI;
        uint32_t      the_channel_restore_cache[IDE3380_channel_count]{};
        volatile bool the_transfer_complete_flag{true};

    public:
        static IDE3380_register_access *optional_one_and_only;

    public:
        explicit IDE3380_register_access(Abstract::Abstract_SPI &SPI) : use_SPI(SPI) {
        }

        void initialize();

        uint32_t SPI_read_register(uint8_t register_address);

        uint32_t SPI_update_register(uint8_t register_address, uint8_t nRead_Write, uint32_t data);

        void override_all_thresholds();

        void disable_all_channels();

        void restore_all_channels();

        void disable_channel(uint8_t index);

        void enable_channel(uint8_t index);

        uint32_t set_channel_threshold(uint8_t index, uint8_t threshold);

        void dump();

        void print_diag();

        void transfer_complete() { the_transfer_complete_flag = true; }
    };
}
