/*
* Copyright (C) 2025-2026 Integrated Detector Electronics AS
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
* @file   Default_configuration.h
* @author AndersEmilOlsen, IDEAS
* @date   02.02.2026
* @brief  
*/

#pragma once

import Support.Abstract_configuration;
#include "Persistent_parameter_id.h"
#include "IDE3380_register_decoder.h"

namespace Application {
    using namespace Repository ;

    class Default_configuration : public Abstract_configuration {
        enum {
            Default_mode = Mode_demo,
            Default_reserved = 0,
            Default_channel = 18,
            Default_format = Format_simple_R6,
            Default_pedestal = 50,
            Default_gain = 1,

            Default_ADC_cal_mV_35V = -35000,
            Default_ADC_cal_mV_40V = -40000,
            Default_DAC_cal_mV_35V = -35000,
            Default_DAC_cal_mV_40V = -40000,

            Default_A = 100000,
            Default_B = 10000,
            Default_cadence = 2,
            Default_bias = 0,


            // Default summing channel configuration:
            // cal_select_channel = 0 (13.1, Calibration Test Enable)
            // qc_threshold = 255 (5.8, Charge comparator threshold)
            // qc_hysteresis = 7 (2.3, Charge comparator hysteresis)
            // pu_channel = 0 (1.1, Power up channel)
            // enable_triggering = 0 (0.1, Enable channel trigger outputs)
            Default_summing_channel = 0x0000'03C3,

            Default_clear_register = 0,
        };

    private:
        constexpr static Attribute_type the_attributes[] = {
            {Not_a_legal_id, State_void, 4, Default_reserved, nullptr},

            {Serial_number, State_void, 4, 0, "Serial_number"},

            {IDE3380_0, State_void, 4, Default_input_channel_config, "IDE3380_0"},
            {IDE3380_1, State_void, 4, Default_input_channel_config, "IDE3380_1"},
            {IDE3380_2, State_void, 4, Default_input_channel_config, "IDE3380_2"},
            {IDE3380_3, State_void, 4, Default_input_channel_config, "IDE3380_3"},
            {IDE3380_4, State_void, 4, Default_input_channel_config, "IDE3380_4"},
            {IDE3380_5, State_void, 4, Default_input_channel_config, "IDE3380_5"},
            {IDE3380_6, State_void, 4, Default_input_channel_config, "IDE3380_6"},
            {IDE3380_7, State_void, 4, Default_input_channel_config, "IDE3380_7"},
            {IDE3380_8, State_void, 4, Default_input_channel_config, "IDE3380_8"},
            {IDE3380_9, State_void, 4, Default_input_channel_config, "IDE3380_9"},
            {IDE3380_10, State_void, 4, Default_input_channel_config, "IDE3380_10"},
            {IDE3380_11, State_void, 4, Default_input_channel_config, "IDE3380_11"},
            {IDE3380_12, State_void, 4, Default_input_channel_config, "IDE3380_12"},
            {IDE3380_13, State_void, 4, Default_input_channel_config, "IDE3380_13"},
            {IDE3380_14, State_void, 4, Default_input_channel_config, "IDE3380_14"},
            {IDE3380_15, State_void, 4, Default_input_channel_config, "IDE3380_15"},
            {IDE3380_16, State_void, 4, Default_summing_channel, "IDE3380_16"},
            {IDE3380_17, State_void, 4, 0x00019494, "IDE3380_17"},
            {IDE3380_18, State_void, 4, 0x00000028, "IDE3380_18"},
            {IDE3380_19, State_void, 4, 0x00000009, "IDE3380_19"},
            {IDE3380_20, State_void, 4, Default_clear_register, "IDE3380_20"},
            {IDE3380_21, State_void, 4, 0x0003FFED, "IDE3380_21"},
            {IDE3380_22, State_void, 4, 0x00000008, "IDE3380_22"},
            {IDE3380_23, State_void, 4, 0x0003FFFE, "IDE3380_23"},
            {IDE3380_24, State_void, 4, 0x00007D61, "IDE3380_24"},
            {IDE3380_25, State_void, 4, Default_clear_register, "IDE3380_25"},
            {IDE3380_26, State_void, 4, Default_clear_register, "IDE3380_26"},
            {IDE3380_27, State_void, 4, Default_clear_register, "IDE3380_27"},
            {IDE3380_28, State_void, 4, Default_clear_register, "IDE3380_28"},
            {IDE3380_29, State_void, 4, Default_clear_register, "IDE3380_29"},

            {Reserved_id_1, State_void, 4, Default_reserved, nullptr},
            {Reserved_id_2, State_void, 4, Default_reserved, nullptr},
            {Reserved_id_3, State_void, 4, Default_reserved, nullptr},
            {Active_mode, State_void, 4, Default_mode, "Active_mode"},

            {Cadence_demo, State_void, 4, Default_cadence, "Cadence_demo"},
            {Cadence_nominal, State_void, 4, Default_cadence, "Cadence_nominal"},
            {Cadence_idle, State_void, 4, Default_cadence, "Cadence_idle"},
            {Cadence_reserved, State_void, 4, Default_reserved, nullptr},

            {Channel_demo, State_void, 4, Default_channel, "Channel_demo"},
            {Channel_nominal, State_void, 4, Default_channel, "Channel_nominal"},
            {Channel_idle, State_void, 4, Default_channel, "Channel_idle"},
            {Channel_reserved, State_void, 4, Default_reserved, nullptr},

            {Format_demo, State_void, 4, Default_format, "Format_demo"},
            {Format_idle, State_void, 4, Default_format, "Format_idle"},
            {Format_nominal, State_void, 4, Default_format, "Format_nominal"},
            {Format_reserved, State_void, 4, Default_reserved, nullptr},

            {Cal_parameter_A, State_void, 4, Default_A, "A"},
            {Cal_parameter_B, State_void, 4, Default_B, "B"},
            {Cal_bias_25C, State_void, 4, Default_bias, "Bias_25C"},
            {Cal_reserved, State_void, 4, Default_reserved, nullptr},

            {Cal_ADC_35V, State_void, 4, Default_ADC_cal_mV_35V, "Cal_ADC_35V"},
            {Cal_ADC_40V, State_void, 4, Default_ADC_cal_mV_40V, "Cal_ADC_40V"},
            {Cal_DAC_35V, State_void, 4, Default_DAC_cal_mV_35V, "Cal_DAC_35V"},
            {Cal_DAC_40V, State_void, 4, Default_DAC_cal_mV_40V, "Cal_DAC_40V"},

            {Cal_integration_time, State_void, 4, 100, "Integration_time"},
            {Cal_start_threshold, State_void, 4, 100, "Start_threshold"},
            {Cal_stop_count, State_void, 4, 100, "Stop_count"},
            {Cal_readout_count, State_void, 4, 100000, "Readout_count"},

            {Cal_input_1_offset, State_void, 4, Default_pedestal, "Pedestal_1"},
            {Cal_input_2_offset, State_void, 4, Default_pedestal, "Pedestal_2"},
            {Cal_input_3_offset, State_void, 4, Default_pedestal, "Pedestal_3"},
            {Cal_input_4_offset, State_void, 4, Default_pedestal, "Pedestal_4"},
            {Cal_input_5_offset, State_void, 4, Default_pedestal, "Pedestal_5"},
            {Cal_input_6_offset, State_void, 4, Default_pedestal, "Pedestal_6"},
            {Cal_input_7_offset, State_void, 4, Default_pedestal, "Pedestal_7"},
            {Cal_input_8_offset, State_void, 4, Default_pedestal, "Pedestal_8"},
            {Cal_input_9_offset, State_void, 4, Default_pedestal, "Pedestal_9"},
            {Cal_input_10_offset, State_void, 4, Default_pedestal, "Pedestal_10"},
            {Cal_input_11_offset, State_void, 4, Default_pedestal, "Pedestal_11"},
            {Cal_input_12_offset, State_void, 4, Default_pedestal, "Pedestal_12"},
            {Cal_input_13_offset, State_void, 4, Default_pedestal, "Pedestal_13"},
            {Cal_input_14_offset, State_void, 4, Default_pedestal, "Pedestal_14"},
            {Cal_input_15_offset, State_void, 4, Default_pedestal, "Pedestal_15"},
            {Cal_input_16_offset, State_void, 4, Default_pedestal, "Pedestal_16"},

            {Cal_input_1_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_2_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_3_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_4_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_5_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_6_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_7_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_8_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_9_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_10_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_11_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_12_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_13_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_14_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_15_gain, State_void, 4, Default_gain, nullptr},
            {Cal_input_16_gain, State_void, 4, Default_gain, nullptr},

        };

        constexpr static uint32_t the_attribute_count = sizeof(the_attributes) / sizeof(Attribute_type);

    public:
        [[nodiscard]] uint32_t get_count() const override { return the_attribute_count; };

        [[nodiscard]] const Attribute_type &get_attribute(uint32_t index) const override {
            if (index < the_attribute_count) { return the_attributes[index]; }
            return the_attributes[0];
        }
    };
} // Application
