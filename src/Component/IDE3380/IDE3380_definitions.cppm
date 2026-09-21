/*
* Copyright (C) 2020-2025 Integrated Detector Electronics AS
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
* @file   IDE3380_definitions.h
* @author AndersEmilOlsen, IDEAS
* @date   13.05.2026
* @brief  
*/

module;
#include <cstdint>

export module Component.IDE3380_definitions;



export namespace IDE3380 {
    enum {
        IDE3380_read_only = 0,
        IDE3380_write_read = 1,
        IDE3380_write_only = 2,
        IDE3380_ADC_bits = 12,
        IDE3380_ADC_range = 1 << IDE3380_ADC_bits,
        IDE3380_first_channel = 0,
        IDE3380_last_channel = 15,
        IDE3380_input_count = 16,
        IDE3380_summing_channel = 16,
        IDE3380_channel_count = 17,
        IDE3380_register_count = 33,
        IDE3380_readout_iterations = 10,
    };

    static_assert(static_cast<int>(IDE3380_ADC_range) == 4096, "Size missmatch");

    struct TXD_data_t {
        uint8_t trigger_flag;
        uint8_t ADC_Channel;
        uint8_t trigger;
        uint16_t ADC_value;
    };

    using wakeup_func = void (*)(void *);
    using readout_func = void (*)(void *, const TXD_data_t &data);

}
