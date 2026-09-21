/*
* Copyright (C) 2025-2025 Integrated Detector Electronics AS
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
* @file   IDE3380_register.h
* @author AndersEmilOlsen, IDEAS
* @date   17.03.2026
* @brief  
*/
#pragma once

#include <cstdint>

#include "IDE3380_interface.h"

namespace IDE3380 {
    constexpr uint8_t IDE3380_register_width[IDE3380_register_count] = {
        26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
        14, 24, 23, 6, 8, 18, 6, 19, 15, 6, 2, 2, 1, 1, 17, 12, 28
    };

    enum {
        IDE3380_channel_1_reg = 0x00,
        IDE3380_channel_2_reg = 0x01,
        IDE3380_channel_3_reg = 0x02,
        IDE3380_channel_4_reg = 0x03,
        IDE3380_channel_5_reg = 0x04,
        IDE3380_channel_6_reg = 0x05,
        IDE3380_channel_7_reg = 0x06,
        IDE3380_channel_8_reg = 0x07,
        IDE3380_channel_9_reg = 0x08,
        IDE3380_channel_10_reg = 0x09,
        IDE3380_channel_11_reg = 0x0A,
        IDE3380_channel_12_reg = 0x0B,
        IDE3380_channel_13_reg = 0x0C,
        IDE3380_channel_14_reg = 0x0D,
        IDE3380_channel_15_reg = 0x0E,
        IDE3380_channel_16_reg = 0x0F,
        IDE3380_channel_17_reg = 0x10,
        IDE3380_channel_config_reg = 0x11,
        IDE3380_channel_control_reg = 0x12,
        IDE3380_ADC_config_reg = 0x13,
        IDE3380_cal_DAC_reg = 0x14,
        IDE3380_PD_modules_reg = 0x15,
        IDE3380_cal_control_reg = 0x16,
        IDE3380_readout_fixed_list_reg = 0x17,
        IDE3380_readout_mode_reg = 0x18,
        IDE3380_analog_mux_control_reg = 0x19,
        IDE3380_ADC_clock_div_reg = 0x1A,
        IDE3380_sysclock_control_reg = 0x1B,
        IDE3380_cmd_DCAL_PU_reg = 0x1C,
        IDE3380_cmd_readout_PU_reg = 0x1D,
        IDE3380_trigger_latches_RO_reg = 0x1E,
        IDE3380_ADC_out_RO_reg = 0x1F,
        IDE3380_parity_error_RO_reg = 0x20,
    };

    enum Readout_mode_value {
        Disable_external_hold = 32097,
        Enable_external_hold = 32098,
    };

    // Default input channel configuration:
    // cmis_detector_voffset = 0 (18.8, AIN input voltage offset)
    // cmis_detector_ioffset = 0 (15.3, AIN input current offset)
    // cmis_impedance_reduction = 0 (14.1, AIN input impedance (VBIAS. 0=5uA, 1=10uA))
    // cal_select_channel = 0 (13.1, Calibration Test Enable)
    // qc_threshold = 255 (5.8, Charge comparator threshold)
    // qc_hysteresis = 7 (2.3, Charge comparator hysteresis)
    // pu_channel = 0 (1.1, Power up channel)
    // enable_triggering = 0 (0.1, Enable channel trigger outputs)
    enum IDE3380_channel_control_register{
        CMIS_voffset_pos = 18,
        Center_voffset = 128 << CMIS_voffset_pos,
        QC_threshold_pos = 5,
        QC_threshold_mask = 0xFF << QC_threshold_pos,
        Default_threshold = 25,
        QC_default_threshold = Default_threshold << QC_threshold_pos,
        PU_CHANNEL = 1 << 1,
        EN_TRIGGER = 1 << 0,
        Disable_channel_setting = QC_threshold_mask,
        Enable_channel_and_max_threshold = Center_voffset | QC_threshold_mask | PU_CHANNEL | EN_TRIGGER,
        Default_input_channel_config = Center_voffset | QC_default_threshold | PU_CHANNEL | EN_TRIGGER,
    };

    class IDE3380_register_decoder {
        uint32_t the_value{};

    public:
        void dump(uint32_t address, uint32_t value);

        void dump_field(const char *name, int pos, int width, const char *brief = nullptr) const;

        [[nodiscard]] int field_value(int pos, int width) const;

        static int field_value(uint32_t value, int pos, int width);

        static void dump_decoded(const uint32_t *data, uint32_t count);

    private:
        static void dump_decoded(const char *name, int pos, int width, const uint32_t *data, uint32_t count);
    };
} // Application
