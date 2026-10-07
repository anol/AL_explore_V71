

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
*
*/


/*
*    Please note: the content of this file was generated using XSLT.
*
*                 D O   N O T   E D I T
*/

module Domain.SpectraNode_keyword_lookup;

namespace SpectraNode_interface {

    constexpr char const *the_keywords[number_of_keys] = {
        "!", // No_key
        "a", // Key_a
        "active", // Key_active
        "adc", // Key_adc
        "ADC", // Key_ADC
        "ALARM", // Key_ALARM
        "APPLY", // Key_APPLY
        "ASIC", // Key_ASIC
        "asic_nbr", // Key_asic_nbr
        "b", // Key_b
        "bias", // Key_bias
        "BIAS", // Key_BIAS
        "CADENCE", // Key_CADENCE
        "CAL", // Key_CAL
        "cal_35V", // Key_cal_35V
        "cal_40V", // Key_cal_40V
        "calibration", // Key_calibration
        "channel", // Key_channel
        "CHANNEL", // Key_CHANNEL
        "CLEAN", // Key_CLEAN
        "CONFIG", // Key_CONFIG
        "Configuration_manager", // Key_Configuration_manager
        "CPS", // Key_CPS
        "CSV", // Key_CSV
        "dac", // Key_dac
        "DAC", // Key_DAC
        "data16", // Key_data16
        "data32", // Key_data32
        "data8", // Key_data8
        "date", // Key_date
        "demo", // Key_demo
        "DEMO", // Key_DEMO
        "device", // Key_device
        "DIAG", // Key_DIAG
        "DUMP", // Key_DUMP
        "ECHO", // Key_ECHO
        "format", // Key_format
        "FORMAT", // Key_FORMAT
        "gain", // Key_gain
        "GET", // Key_GET
        "HELP", // Key_HELP
        "Housekeeping", // Key_Housekeeping
        "idle", // Key_idle
        "IDLE", // Key_IDLE
        "INPUT", // Key_INPUT
        "Instrument_calibration", // Key_Instrument_calibration
        "integration_time", // Key_integration_time
        "LOAD", // Key_LOAD
        "micro_sievert", // Key_micro_sievert
        "millis", // Key_millis
        "MODE", // Key_MODE
        "Mode_control", // Key_Mode_control
        "N42", // Key_N42
        "NOISE", // Key_NOISE
        "nominal", // Key_nominal
        "NOMINAL", // Key_NOMINAL
        "offset", // Key_offset
        "OFFSET", // Key_OFFSET
        "parameter", // Key_parameter
        "pedestal", // Key_pedestal
        "PEDESTAL", // Key_PEDESTAL
        "R6", // Key_R6
        "readout_count", // Key_readout_count
        "REG", // Key_REG
        "reg_addr", // Key_reg_addr
        "RESET", // Key_RESET
        "SAVE", // Key_SAVE
        "seconds", // Key_seconds
        "serial_number", // Key_serial_number
        "simple_R6", // Key_simple_R6
        "Spectroscopic_data", // Key_Spectroscopic_data
        "start_threshold", // Key_start_threshold
        "STATUS", // Key_STATUS
        "stop_count", // Key_stop_count
        "TEST", // Key_TEST
        "time", // Key_time
        "TIME", // Key_TIME
        "TRACE", // Key_TRACE
        "V35", // Key_V35
        "V40", // Key_V40
        "VBIAS", // Key_VBIAS
        "VERSION", // Key_VERSION
    };

    const char *get_keyword(unsigned char key) {
        if (key < number_of_keys) return the_keywords[key];
        else return "?";
    }

}
