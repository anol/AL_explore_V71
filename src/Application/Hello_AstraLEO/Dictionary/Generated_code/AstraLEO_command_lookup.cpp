

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


module;
#include <cstdint>

module Application.AstraLEO_command_lookup;
import Domain.Keyword_lookup;
import Application.AstraLEO_command_help;

using namespace Instruction;

using namespace Domain_interface;

namespace AstraLEO_interface{

    const Instruction_token token_DEMO_CADENCE_seconds[] = {
        {help_DEMO_CADENCE_seconds, Cmd_DEMO_CADENCE_seconds, Key_Mode_control},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_DEMO_CADENCE[] = {
        {Integer_token, Key_seconds, token_DEMO_CADENCE_seconds, 0, static_cast<int32_t>(1000000)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_DEMO_CHANNEL_channel[] = {
        {help_DEMO_CHANNEL_channel, Cmd_DEMO_CHANNEL_channel, Key_Mode_control},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_DEMO_CHANNEL[] = {
        {Integer_token, Key_channel, token_DEMO_CHANNEL_channel, 0, static_cast<int32_t>(18)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_DEMO[] = {
        {Keyword_token, "CADENCE", Key_CADENCE, (uint32_t)0, token_DEMO_CADENCE},
        {Keyword_token, "CHANNEL", Key_CHANNEL, (uint32_t)0, token_DEMO_CHANNEL},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_MODE_CAL[] = {
        {help_MODE_CAL, Cmd_MODE_CAL, Key_Mode_control},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_MODE_DEMO[] = {
        {help_MODE_DEMO, Cmd_MODE_DEMO, Key_Mode_control},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_MODE_IDLE[] = {
        {help_MODE_IDLE, Cmd_MODE_IDLE, Key_Mode_control},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_MODE_NOMINAL[] = {
        {help_MODE_NOMINAL, Cmd_MODE_NOMINAL, Key_Mode_control},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_MODE[] = {
        {Keyword_token, "CAL", Key_CAL, (uint32_t)0, token_MODE_CAL},
        {Keyword_token, "DEMO", Key_DEMO, (uint32_t)0, token_MODE_DEMO},
        {Keyword_token, "IDLE", Key_IDLE, (uint32_t)0, token_MODE_IDLE},
        {Keyword_token, "NOMINAL", Key_NOMINAL, (uint32_t)0, token_MODE_NOMINAL},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_NOMINAL_ALARM_micro_sievert[] = {
        {help_NOMINAL_ALARM_micro_sievert, Cmd_NOMINAL_ALARM_micro_sievert, Key_Mode_control},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_NOMINAL_ALARM[] = {
        {Integer_token, Key_micro_sievert, token_NOMINAL_ALARM_micro_sievert, 0, static_cast<int32_t>(1000000)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_NOMINAL_CADENCE_seconds[] = {
        {help_NOMINAL_CADENCE_seconds, Cmd_NOMINAL_CADENCE_seconds, Key_Mode_control},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_NOMINAL_CADENCE[] = {
        {Integer_token, Key_seconds, token_NOMINAL_CADENCE_seconds, 0, static_cast<int32_t>(1000000)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_NOMINAL[] = {
        {Keyword_token, "ALARM", Key_ALARM, (uint32_t)0, token_NOMINAL_ALARM},
        {Keyword_token, "CADENCE", Key_CADENCE, (uint32_t)0, token_NOMINAL_CADENCE},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_FORMAT_CPS[] = {
        {help_FORMAT_CPS, Cmd_FORMAT_CPS, Key_Science_data},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_FORMAT_N42[] = {
        {help_FORMAT_N42, Cmd_FORMAT_N42, Key_Science_data},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_FORMAT_R6[] = {
        {help_FORMAT_R6, Cmd_FORMAT_R6, Key_Science_data},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_FORMAT[] = {
        {Keyword_token, "CPS", Key_CPS, (uint32_t)0, token_FORMAT_CPS},
        {Keyword_token, "N42", Key_N42, (uint32_t)0, token_FORMAT_N42},
        {Keyword_token, "R6", Key_R6, (uint32_t)0, token_FORMAT_R6},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_GET_RESET[] = {
        {help_GET_RESET, Cmd_GET_RESET, Key_Science_data},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_GET[] = {
        {Keyword_token, "RESET", Key_RESET, (uint32_t)0, token_GET_RESET},
        {help_GET, Cmd_GET, Key_Science_data},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_TIME_date_time[] = {
        {help_TIME_date_time, Cmd_TIME_date_time, Key_Science_data},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_TIME_date[] = {
        {Integer_token, Key_time, token_TIME_date_time, 0, static_cast<int32_t>(1000000)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_TIME[] = {
        {Integer_token, Key_date, token_TIME_date, 0, static_cast<int32_t>(0x7FFFFFFF)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_ADC_SET_low_high[] = {
        {help_BIAS_ADC_SET_low_high, Cmd_BIAS_ADC_SET_low_high, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_ADC_SET_low[] = {
        {Integer_token, Key_high, token_BIAS_ADC_SET_low_high, 0, static_cast<int32_t>(4095)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_ADC_SET[] = {
        {Integer_token, Key_low, token_BIAS_ADC_SET_low, 0, static_cast<int32_t>(4095)},
        {help_BIAS_ADC_SET, Cmd_BIAS_ADC_SET, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_ADC[] = {
        {Keyword_token, "SET", Key_SET, (uint32_t)0, token_BIAS_ADC_SET},
        {help_BIAS_ADC, Cmd_BIAS_ADC, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_C25_bias_mV[] = {
        {help_BIAS_C25_bias_mV, Cmd_BIAS_C25_bias_mV, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_C25[] = {
        {Integer_token, Key_bias_mV, token_BIAS_C25_bias_mV, -50000, static_cast<int32_t>(0)},
        {help_BIAS_C25, Cmd_BIAS_C25, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_CAL_low_mV_high_mV[] = {
        {help_BIAS_CAL_low_mV_high_mV, Cmd_BIAS_CAL_low_mV_high_mV, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_CAL_low_mV[] = {
        {Integer_token, Key_high_mV, token_BIAS_CAL_low_mV_high_mV, -50000, static_cast<int32_t>(0)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_CAL[] = {
        {Integer_token, Key_low_mV, token_BIAS_CAL_low_mV, -50000, static_cast<int32_t>(0)},
        {help_BIAS_CAL, Cmd_BIAS_CAL, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_DAC_data12[] = {
        {help_BIAS_DAC_data12, Cmd_BIAS_DAC_data12, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_DAC_SET_low_high[] = {
        {help_BIAS_DAC_SET_low_high, Cmd_BIAS_DAC_SET_low_high, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_DAC_SET_low[] = {
        {Integer_token, Key_high, token_BIAS_DAC_SET_low_high, 0, static_cast<int32_t>(4095)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_DAC_SET[] = {
        {Integer_token, Key_low, token_BIAS_DAC_SET_low, 0, static_cast<int32_t>(4095)},
        {help_BIAS_DAC_SET, Cmd_BIAS_DAC_SET, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_DAC[] = {
        {Integer_token, Key_data12, token_BIAS_DAC_data12, 0, static_cast<int32_t>(4095)},
        {Keyword_token, "SET", Key_SET, (uint32_t)0, token_BIAS_DAC_SET},
        {help_BIAS_DAC, Cmd_BIAS_DAC, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_DIAG[] = {
        {help_BIAS_DIAG, Cmd_BIAS_DIAG, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_ERROR[] = {
        {help_BIAS_ERROR, Cmd_BIAS_ERROR, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_SET_bias_mV[] = {
        {help_BIAS_SET_bias_mV, Cmd_BIAS_SET_bias_mV, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_SET[] = {
        {Integer_token, Key_bias_mV, token_BIAS_SET_bias_mV, -50000, static_cast<int32_t>(0)},
        {help_BIAS_SET, Cmd_BIAS_SET, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_TEMP_temp[] = {
        {help_BIAS_TEMP_temp, Cmd_BIAS_TEMP_temp, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_TEMP_SET_a_b[] = {
        {help_BIAS_TEMP_SET_a_b, Cmd_BIAS_TEMP_SET_a_b, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_TEMP_SET_a[] = {
        {Integer_token, Key_b, token_BIAS_TEMP_SET_a_b, -1000000, static_cast<int32_t>(1000000)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_TEMP_SET[] = {
        {Integer_token, Key_a, token_BIAS_TEMP_SET_a, -1000000, static_cast<int32_t>(1000000)},
        {help_BIAS_TEMP_SET, Cmd_BIAS_TEMP_SET, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_TEMP[] = {
        {Integer_token, Key_temp, token_BIAS_TEMP_temp, -10000, static_cast<int32_t>(+10000)},
        {Keyword_token, "SET", Key_SET, (uint32_t)0, token_BIAS_TEMP_SET},
        {help_BIAS_TEMP, Cmd_BIAS_TEMP, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS_VOLT[] = {
        {help_BIAS_VOLT, Cmd_BIAS_VOLT, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_BIAS[] = {
        {Keyword_token, "ADC", Key_ADC, (uint32_t)0, token_BIAS_ADC},
        {Keyword_token, "C25", Key_C25, (uint32_t)0, token_BIAS_C25},
        {Keyword_token, "CAL", Key_CAL, (uint32_t)0, token_BIAS_CAL},
        {Keyword_token, "DAC", Key_DAC, (uint32_t)0, token_BIAS_DAC},
        {Keyword_token, "DIAG", Key_DIAG, (uint32_t)0, token_BIAS_DIAG},
        {Keyword_token, "ERROR", Key_ERROR, (uint32_t)0, token_BIAS_ERROR},
        {Keyword_token, "SET", Key_SET, (uint32_t)0, token_BIAS_SET},
        {Keyword_token, "TEMP", Key_TEMP, (uint32_t)0, token_BIAS_TEMP},
        {Keyword_token, "VOLT", Key_VOLT, (uint32_t)0, token_BIAS_VOLT},
        {help_BIAS, Cmd_BIAS, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CAL_DIAG[] = {
        {help_CAL_DIAG, Cmd_CAL_DIAG, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CAL_DUMP[] = {
        {help_CAL_DUMP, Cmd_CAL_DUMP, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CAL_TEST_BIAS[] = {
        {help_CAL_TEST_BIAS, Cmd_CAL_TEST_BIAS, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CAL_TEST_NOISE[] = {
        {help_CAL_TEST_NOISE, Cmd_CAL_TEST_NOISE, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CAL_TEST_OFFSET[] = {
        {help_CAL_TEST_OFFSET, Cmd_CAL_TEST_OFFSET, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CAL_TEST_PEDESTAL[] = {
        {help_CAL_TEST_PEDESTAL, Cmd_CAL_TEST_PEDESTAL, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CAL_TEST[] = {
        {Keyword_token, "BIAS", Key_BIAS, (uint32_t)0, token_CAL_TEST_BIAS},
        {Keyword_token, "NOISE", Key_NOISE, (uint32_t)0, token_CAL_TEST_NOISE},
        {Keyword_token, "OFFSET", Key_OFFSET, (uint32_t)0, token_CAL_TEST_OFFSET},
        {Keyword_token, "PEDESTAL", Key_PEDESTAL, (uint32_t)0, token_CAL_TEST_PEDESTAL},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CAL_TRACE[] = {
        {help_CAL_TRACE, Cmd_CAL_TRACE, Key_Instrument_calibration},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CAL[] = {
        {Keyword_token, "DIAG", Key_DIAG, (uint32_t)0, token_CAL_DIAG},
        {Keyword_token, "DUMP", Key_DUMP, (uint32_t)0, token_CAL_DUMP},
        {Keyword_token, "TEST", Key_TEST, (uint32_t)0, token_CAL_TEST},
        {Keyword_token, "TRACE", Key_TRACE, (uint32_t)0, token_CAL_TRACE},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_ASIC_asic_nbr[] = {
        {help_ASIC_asic_nbr, Cmd_ASIC_asic_nbr, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_ASIC_DUMP[] = {
        {help_ASIC_DUMP, Cmd_ASIC_DUMP, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_ASIC_LOAD[] = {
        {help_ASIC_LOAD, Cmd_ASIC_LOAD, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_ASIC_REG_reg_addr_data32[] = {
        {help_ASIC_REG_reg_addr_data32, Cmd_ASIC_REG_reg_addr_data32, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_ASIC_REG_reg_addr[] = {
        {Integer_token, Key_data32, token_ASIC_REG_reg_addr_data32, 0, static_cast<int32_t>(0x7FFFFFFF)},
        {help_ASIC_REG_reg_addr, Cmd_ASIC_REG_reg_addr, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_ASIC_REG[] = {
        {Integer_token, Key_reg_addr, token_ASIC_REG_reg_addr, 0, static_cast<int32_t>(32)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_ASIC[] = {
        {Integer_token, Key_asic_nbr, token_ASIC_asic_nbr, 1, static_cast<int32_t>(5)},
        {Keyword_token, "DUMP", Key_DUMP, (uint32_t)0, token_ASIC_DUMP},
        {Keyword_token, "LOAD", Key_LOAD, (uint32_t)0, token_ASIC_LOAD},
        {Keyword_token, "REG", Key_REG, (uint32_t)0, token_ASIC_REG},
        {help_ASIC, Cmd_ASIC, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CONFIG_offset_data32[] = {
        {help_CONFIG_offset_data32, Cmd_CONFIG_offset_data32, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CONFIG_offset[] = {
        {Integer_token, Key_data32, token_CONFIG_offset_data32, 0, static_cast<int32_t>(0x7FFFFFFF)},
        {help_CONFIG_offset, Cmd_CONFIG_offset, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CONFIG_APPLY[] = {
        {help_CONFIG_APPLY, Cmd_CONFIG_APPLY, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CONFIG_ASIC_asic_nbr_reg_addr_data32[] = {
        {help_CONFIG_ASIC_asic_nbr_reg_addr_data32, Cmd_CONFIG_ASIC_asic_nbr_reg_addr_data32, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CONFIG_ASIC_asic_nbr_reg_addr[] = {
        {Integer_token, Key_data32, token_CONFIG_ASIC_asic_nbr_reg_addr_data32, 0, static_cast<int32_t>(0x7FFFFFFF)},
        {help_CONFIG_ASIC_asic_nbr_reg_addr, Cmd_CONFIG_ASIC_asic_nbr_reg_addr, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CONFIG_ASIC_asic_nbr[] = {
        {Integer_token, Key_reg_addr, token_CONFIG_ASIC_asic_nbr_reg_addr, 0, static_cast<int32_t>(32)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CONFIG_ASIC[] = {
        {Integer_token, Key_asic_nbr, token_CONFIG_ASIC_asic_nbr, 1, static_cast<int32_t>(5)},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CONFIG_CLEAN[] = {
        {help_CONFIG_CLEAN, Cmd_CONFIG_CLEAN, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CONFIG_DUMP[] = {
        {help_CONFIG_DUMP, Cmd_CONFIG_DUMP, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CONFIG_LOAD[] = {
        {help_CONFIG_LOAD, Cmd_CONFIG_LOAD, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CONFIG_SAVE[] = {
        {help_CONFIG_SAVE, Cmd_CONFIG_SAVE, Key_Configuration_manager},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_CONFIG[] = {
        {Integer_token, Key_offset, token_CONFIG_offset, 0, static_cast<int32_t>(4095)},
        {Keyword_token, "APPLY", Key_APPLY, (uint32_t)0, token_CONFIG_APPLY},
        {Keyword_token, "ASIC", Key_ASIC, (uint32_t)0, token_CONFIG_ASIC},
        {Keyword_token, "CLEAN", Key_CLEAN, (uint32_t)0, token_CONFIG_CLEAN},
        {Keyword_token, "DUMP", Key_DUMP, (uint32_t)0, token_CONFIG_DUMP},
        {Keyword_token, "LOAD", Key_LOAD, (uint32_t)0, token_CONFIG_LOAD},
        {Keyword_token, "SAVE", Key_SAVE, (uint32_t)0, token_CONFIG_SAVE},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_DIAG[] = {
        {help_DIAG, Cmd_DIAG, Key_Housekeeping},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_ECHO[] = {
        {help_ECHO, Cmd_ECHO, Key_Housekeeping},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_HELP[] = {
        {help_HELP, Cmd_HELP, Key_Housekeeping},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_HOUSE[] = {
        {help_HOUSE, Cmd_HOUSE, Key_Housekeeping},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_STATUS[] = {
        {help_STATUS, Cmd_STATUS, Key_Housekeeping},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_TEST[] = {
        {help_TEST, Cmd_TEST, Key_Housekeeping},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_TRACE[] = {
        {help_TRACE, Cmd_TRACE, Key_Housekeeping},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token token_VERSION[] = {
        {help_VERSION, Cmd_VERSION, Key_Housekeeping},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token t_main[] = {
        {Keyword_token, "DEMO", Key_DEMO, (uint32_t)0, token_DEMO},
        {Keyword_token, "MODE", Key_MODE, (uint32_t)0, token_MODE},
        {Keyword_token, "NOMINAL", Key_NOMINAL, (uint32_t)0, token_NOMINAL},
        {Keyword_token, "FORMAT", Key_FORMAT, (uint32_t)0, token_FORMAT},
        {Keyword_token, "GET", Key_GET, (uint32_t)0, token_GET},
        {Keyword_token, "TIME", Key_TIME, (uint32_t)0, token_TIME},
        {Keyword_token, "BIAS", Key_BIAS, (uint32_t)0, token_BIAS},
        {Keyword_token, "CAL", Key_CAL, (uint32_t)0, token_CAL},
        {Keyword_token, "ASIC", Key_ASIC, (uint32_t)0, token_ASIC},
        {Keyword_token, "CONFIG", Key_CONFIG, (uint32_t)0, token_CONFIG},
        {Keyword_token, "DIAG", Key_DIAG, (uint32_t)0, token_DIAG},
        {Keyword_token, "ECHO", Key_ECHO, (uint32_t)0, token_ECHO},
        {Keyword_token, "HELP", Key_HELP, (uint32_t)0, token_HELP},
        {Keyword_token, "HOUSE", Key_HOUSE, (uint32_t)0, token_HOUSE},
        {Keyword_token, "STATUS", Key_STATUS, (uint32_t)0, token_STATUS},
        {Keyword_token, "TEST", Key_TEST, (uint32_t)0, token_TEST},
        {Keyword_token, "TRACE", Key_TRACE, (uint32_t)0, token_TRACE},
        {Keyword_token, "VERSION", Key_VERSION, (uint32_t)0, token_VERSION},
        {End_token, "", No_key, (uint32_t)0, nullptr}
    };

    const Instruction_token * get_commands() { return t_main; }

}
