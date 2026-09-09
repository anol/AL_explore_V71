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
* @file   Persistent_parameter_id.h
* @author AndersEmilOlsen, IDEAS
* @date   05.05.2026
* @brief  
*/

#pragma once

namespace Application {
    enum Operation_mode {
        Mode_idle,
        Mode_nominal,
        Mode_demo,
    };

    enum Data_format {
        Format_no_data,
        Format_legacy_live_view,
        Format_simple_R6,
        Format_only_CPS,
        Format_complex_N42,
        Format_housekeeping,
    };

    enum Channel_number {
        Input_channel_1 = 1,
        Input_channel_16 = 16,
        Analog_summing_ch = 17,
        Digital_summing_ch = 18,
        Number_of_inputs = 16,
        Number_of_channels = 18,
    };

    enum Parameter_id: unsigned char {
        Not_a_legal_id,

        Serial_number,

        IDE3380_0, IDE3380_1, IDE3380_2, IDE3380_3, IDE3380_4,
        IDE3380_5, IDE3380_6, IDE3380_7, IDE3380_8, IDE3380_9,
        IDE3380_10, IDE3380_11, IDE3380_12, IDE3380_13, IDE3380_14,
        IDE3380_15, IDE3380_16, IDE3380_17, IDE3380_18, IDE3380_19,
        IDE3380_20, IDE3380_21, IDE3380_22, IDE3380_23, IDE3380_24,
        IDE3380_25, IDE3380_26, IDE3380_27, IDE3380_28, IDE3380_29,

        Reserved_id_1,
        Reserved_id_2,
        Reserved_id_3,
        Active_mode,

        Cadence_demo,
        Cadence_nominal,
        Cadence_idle,
        Cadence_reserved,

        Channel_demo,
        Channel_nominal,
        Channel_idle,
        Channel_reserved,

        Format_demo,
        Format_idle,
        Format_nominal,
        Format_reserved,

        Cal_parameter_A,
        Cal_parameter_B,
        Cal_bias_25C,
        Cal_reserved,

        Cal_ADC_35V,
        Cal_ADC_40V,
        Cal_DAC_35V,
        Cal_DAC_40V,

        Cal_integration_time,
        Cal_start_threshold,
        Cal_stop_count,
        Cal_readout_count,

        Cal_input_1_offset,
        Cal_input_2_offset,
        Cal_input_3_offset,
        Cal_input_4_offset,
        Cal_input_5_offset,
        Cal_input_6_offset,
        Cal_input_7_offset,
        Cal_input_8_offset,
        Cal_input_9_offset,
        Cal_input_10_offset,
        Cal_input_11_offset,
        Cal_input_12_offset,
        Cal_input_13_offset,
        Cal_input_14_offset,
        Cal_input_15_offset,
        Cal_input_16_offset,

        Cal_input_1_gain,
        Cal_input_2_gain,
        Cal_input_3_gain,
        Cal_input_4_gain,
        Cal_input_5_gain,
        Cal_input_6_gain,
        Cal_input_7_gain,
        Cal_input_8_gain,
        Cal_input_9_gain,
        Cal_input_10_gain,
        Cal_input_11_gain,
        Cal_input_12_gain,
        Cal_input_13_gain,
        Cal_input_14_gain,
        Cal_input_15_gain,
        Cal_input_16_gain,

        Number_of_parameters,
    };
}
