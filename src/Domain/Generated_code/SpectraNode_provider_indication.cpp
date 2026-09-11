

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



#include "SpectraNode_command_lookup.h"
#include "SpectraNode_provider_indication.h"

using namespace Instruction;
using namespace SpectraNode_interface;
        

template<> const Abstract_Mode_control_provider::Lookup_table::Instruction_entry
    Abstract_Mode_control_provider::Lookup_table::instruction_table[] = {
    {&Abstract_Mode_control_provider::p_DEMO_CADENCE_seconds, Cmd_DEMO_CADENCE_seconds},
    {&Abstract_Mode_control_provider::p_DEMO_CHANNEL_channel, Cmd_DEMO_CHANNEL_channel},
    {&Abstract_Mode_control_provider::p_IDLE_CADENCE_seconds, Cmd_IDLE_CADENCE_seconds},
    {&Abstract_Mode_control_provider::p_MODE_DEMO, Cmd_MODE_DEMO},
    {&Abstract_Mode_control_provider::p_MODE_IDLE, Cmd_MODE_IDLE},
    {&Abstract_Mode_control_provider::p_MODE_NOMINAL, Cmd_MODE_NOMINAL},
    {&Abstract_Mode_control_provider::p_NOMINAL_ALARM_micro_sievert, Cmd_NOMINAL_ALARM_micro_sievert},
    {&Abstract_Mode_control_provider::p_NOMINAL_CADENCE_seconds, Cmd_NOMINAL_CADENCE_seconds},
};

namespace SpectraNode_interface{

    bool Abstract_Mode_control_provider::on_indication(Instruction_major &transaction) {
        return Abstract_Mode_control_provider::Lookup_table::lookup(this, transaction);
    }

    void Abstract_Mode_control_provider::p_MODE_DEMO(Instruction_major &transaction){
        v_MODE_DEMO(transaction);
    }
            
    void Abstract_Mode_control_provider::p_MODE_IDLE(Instruction_major &transaction){
        v_MODE_IDLE(transaction);
    }
            
    void Abstract_Mode_control_provider::p_MODE_NOMINAL(Instruction_major &transaction){
        v_MODE_NOMINAL(transaction);
    }
            
    void Abstract_Mode_control_provider::p_IDLE_CADENCE_seconds(Instruction_major &transaction){
        int seconds_2{transaction.get_token_integer(2)};
        v_IDLE_CADENCE_seconds(transaction, seconds_2);
    }
            
    void Abstract_Mode_control_provider::p_DEMO_CADENCE_seconds(Instruction_major &transaction){
        int seconds_2{transaction.get_token_integer(2)};
        v_DEMO_CADENCE_seconds(transaction, seconds_2);
    }
            
    void Abstract_Mode_control_provider::p_DEMO_CHANNEL_channel(Instruction_major &transaction){
        int channel_2{transaction.get_token_integer(2)};
        v_DEMO_CHANNEL_channel(transaction, channel_2);
    }
            
    void Abstract_Mode_control_provider::p_NOMINAL_ALARM_micro_sievert(Instruction_major &transaction){
        int micro_sievert_2{transaction.get_token_integer(2)};
        v_NOMINAL_ALARM_micro_sievert(transaction, micro_sievert_2);
    }
            
    void Abstract_Mode_control_provider::p_NOMINAL_CADENCE_seconds(Instruction_major &transaction){
        int seconds_2{transaction.get_token_integer(2)};
        v_NOMINAL_CADENCE_seconds(transaction, seconds_2);
    }
            
}


template<> const Abstract_Spectroscopic_data_provider::Lookup_table::Instruction_entry
    Abstract_Spectroscopic_data_provider::Lookup_table::instruction_table[] = {
    {&Abstract_Spectroscopic_data_provider::p_FORMAT_CPS, Cmd_FORMAT_CPS},
    {&Abstract_Spectroscopic_data_provider::p_FORMAT_N42, Cmd_FORMAT_N42},
    {&Abstract_Spectroscopic_data_provider::p_FORMAT_R6, Cmd_FORMAT_R6},
    {&Abstract_Spectroscopic_data_provider::p_GET_RESET, Cmd_GET_RESET},
    {&Abstract_Spectroscopic_data_provider::p_GET, Cmd_GET},
    {&Abstract_Spectroscopic_data_provider::p_TIME_date_time, Cmd_TIME_date_time},
};

namespace SpectraNode_interface{

    bool Abstract_Spectroscopic_data_provider::on_indication(Instruction_major &transaction) {
        return Abstract_Spectroscopic_data_provider::Lookup_table::lookup(this, transaction);
    }

    void Abstract_Spectroscopic_data_provider::p_GET_RESET(Instruction_major &transaction){
        v_GET_RESET(transaction);
    }
            
    void Abstract_Spectroscopic_data_provider::p_GET(Instruction_major &transaction){
        v_GET(transaction);
    }
            
    void Abstract_Spectroscopic_data_provider::p_FORMAT_CPS(Instruction_major &transaction){
        v_FORMAT_CPS(transaction);
    }
            
    void Abstract_Spectroscopic_data_provider::p_FORMAT_N42(Instruction_major &transaction){
        v_FORMAT_N42(transaction);
    }
            
    void Abstract_Spectroscopic_data_provider::p_FORMAT_R6(Instruction_major &transaction){
        v_FORMAT_R6(transaction);
    }
            
    void Abstract_Spectroscopic_data_provider::p_TIME_date_time(Instruction_major &transaction){
        int date_1{transaction.get_token_integer(1)};
        int time_2{transaction.get_token_integer(2)};
        v_TIME_date_time(transaction, date_1, time_2);
    }
            
}


template<> const Abstract_Instrument_calibration_provider::Lookup_table::Instruction_entry
    Abstract_Instrument_calibration_provider::Lookup_table::instruction_table[] = {
    {&Abstract_Instrument_calibration_provider::p_CAL_ADC_V35_cal_35V, Cmd_CAL_ADC_V35_cal_35V},
    {&Abstract_Instrument_calibration_provider::p_CAL_ADC_V40_cal_40V, Cmd_CAL_ADC_V40_cal_40V},
    {&Abstract_Instrument_calibration_provider::p_CAL_DAC_V35_cal_35V, Cmd_CAL_DAC_V35_cal_35V},
    {&Abstract_Instrument_calibration_provider::p_CAL_DAC_V40_cal_40V, Cmd_CAL_DAC_V40_cal_40V},
    {&Abstract_Instrument_calibration_provider::p_CAL_DIAG, Cmd_CAL_DIAG},
    {&Abstract_Instrument_calibration_provider::p_CAL_TEST_BIAS, Cmd_CAL_TEST_BIAS},
    {&Abstract_Instrument_calibration_provider::p_CAL_TEST_NOISE, Cmd_CAL_TEST_NOISE},
    {&Abstract_Instrument_calibration_provider::p_CAL_TEST_OFFSET, Cmd_CAL_TEST_OFFSET},
    {&Abstract_Instrument_calibration_provider::p_CAL_TEST_PEDESTAL, Cmd_CAL_TEST_PEDESTAL},
    {&Abstract_Instrument_calibration_provider::p_CAL_TRACE, Cmd_CAL_TRACE},
};

namespace SpectraNode_interface{

    bool Abstract_Instrument_calibration_provider::on_indication(Instruction_major &transaction) {
        return Abstract_Instrument_calibration_provider::Lookup_table::lookup(this, transaction);
    }

    void Abstract_Instrument_calibration_provider::p_CAL_ADC_V35_cal_35V(Instruction_major &transaction){
        int cal_35V_3{transaction.get_token_integer(3)};
        v_CAL_ADC_V35_cal_35V(transaction, cal_35V_3);
    }
            
    void Abstract_Instrument_calibration_provider::p_CAL_ADC_V40_cal_40V(Instruction_major &transaction){
        int cal_40V_3{transaction.get_token_integer(3)};
        v_CAL_ADC_V40_cal_40V(transaction, cal_40V_3);
    }
            
    void Abstract_Instrument_calibration_provider::p_CAL_DAC_V35_cal_35V(Instruction_major &transaction){
        int cal_35V_3{transaction.get_token_integer(3)};
        v_CAL_DAC_V35_cal_35V(transaction, cal_35V_3);
    }
            
    void Abstract_Instrument_calibration_provider::p_CAL_DAC_V40_cal_40V(Instruction_major &transaction){
        int cal_40V_3{transaction.get_token_integer(3)};
        v_CAL_DAC_V40_cal_40V(transaction, cal_40V_3);
    }
            
    void Abstract_Instrument_calibration_provider::p_CAL_DIAG(Instruction_major &transaction){
        v_CAL_DIAG(transaction);
    }
            
    void Abstract_Instrument_calibration_provider::p_CAL_TEST_BIAS(Instruction_major &transaction){
        v_CAL_TEST_BIAS(transaction);
    }
            
    void Abstract_Instrument_calibration_provider::p_CAL_TEST_NOISE(Instruction_major &transaction){
        v_CAL_TEST_NOISE(transaction);
    }
            
    void Abstract_Instrument_calibration_provider::p_CAL_TEST_OFFSET(Instruction_major &transaction){
        v_CAL_TEST_OFFSET(transaction);
    }
            
    void Abstract_Instrument_calibration_provider::p_CAL_TEST_PEDESTAL(Instruction_major &transaction){
        v_CAL_TEST_PEDESTAL(transaction);
    }
            
    void Abstract_Instrument_calibration_provider::p_CAL_TRACE(Instruction_major &transaction){
        v_CAL_TRACE(transaction);
    }
            
}


template<> const Abstract_Configuration_manager_provider::Lookup_table::Instruction_entry
    Abstract_Configuration_manager_provider::Lookup_table::instruction_table[] = {
    {&Abstract_Configuration_manager_provider::p_ASIC_DUMP, Cmd_ASIC_DUMP},
    {&Abstract_Configuration_manager_provider::p_ASIC_LOAD, Cmd_ASIC_LOAD},
    {&Abstract_Configuration_manager_provider::p_ASIC_REG_reg_addr_data32, Cmd_ASIC_REG_reg_addr_data32},
    {&Abstract_Configuration_manager_provider::p_ASIC_REG_reg_addr, Cmd_ASIC_REG_reg_addr},
    {&Abstract_Configuration_manager_provider::p_CONFIG_offset_data32, Cmd_CONFIG_offset_data32},
    {&Abstract_Configuration_manager_provider::p_CONFIG_offset, Cmd_CONFIG_offset},
    {&Abstract_Configuration_manager_provider::p_CONFIG_APPLY, Cmd_CONFIG_APPLY},
    {&Abstract_Configuration_manager_provider::p_CONFIG_CLEAN, Cmd_CONFIG_CLEAN},
    {&Abstract_Configuration_manager_provider::p_CONFIG_DUMP, Cmd_CONFIG_DUMP},
    {&Abstract_Configuration_manager_provider::p_CONFIG_LOAD, Cmd_CONFIG_LOAD},
    {&Abstract_Configuration_manager_provider::p_CONFIG_SAVE, Cmd_CONFIG_SAVE},
};

namespace SpectraNode_interface{

    bool Abstract_Configuration_manager_provider::on_indication(Instruction_major &transaction) {
        return Abstract_Configuration_manager_provider::Lookup_table::lookup(this, transaction);
    }

    void Abstract_Configuration_manager_provider::p_CONFIG_offset_data32(Instruction_major &transaction){
        int offset_1{transaction.get_token_integer(1)};
        int data32_2{transaction.get_token_integer(2)};
        v_CONFIG_offset_data32(transaction, offset_1, data32_2);
    }
            
    void Abstract_Configuration_manager_provider::p_CONFIG_offset(Instruction_major &transaction){
        int offset_1{transaction.get_token_integer(1)};
        v_CONFIG_offset(transaction, offset_1);
    }
            
    void Abstract_Configuration_manager_provider::p_CONFIG_APPLY(Instruction_major &transaction){
        v_CONFIG_APPLY(transaction);
    }
            
    void Abstract_Configuration_manager_provider::p_CONFIG_CLEAN(Instruction_major &transaction){
        v_CONFIG_CLEAN(transaction);
    }
            
    void Abstract_Configuration_manager_provider::p_CONFIG_DUMP(Instruction_major &transaction){
        v_CONFIG_DUMP(transaction);
    }
            
    void Abstract_Configuration_manager_provider::p_CONFIG_LOAD(Instruction_major &transaction){
        v_CONFIG_LOAD(transaction);
    }
            
    void Abstract_Configuration_manager_provider::p_CONFIG_SAVE(Instruction_major &transaction){
        v_CONFIG_SAVE(transaction);
    }
            
    void Abstract_Configuration_manager_provider::p_ASIC_DUMP(Instruction_major &transaction){
        v_ASIC_DUMP(transaction);
    }
            
    void Abstract_Configuration_manager_provider::p_ASIC_LOAD(Instruction_major &transaction){
        v_ASIC_LOAD(transaction);
    }
            
    void Abstract_Configuration_manager_provider::p_ASIC_REG_reg_addr_data32(Instruction_major &transaction){
        int reg_addr_2{transaction.get_token_integer(2)};
        int data32_3{transaction.get_token_integer(3)};
        v_ASIC_REG_reg_addr_data32(transaction, reg_addr_2, data32_3);
    }
            
    void Abstract_Configuration_manager_provider::p_ASIC_REG_reg_addr(Instruction_major &transaction){
        int reg_addr_2{transaction.get_token_integer(2)};
        v_ASIC_REG_reg_addr(transaction, reg_addr_2);
    }
            
}


template<> const Abstract_Housekeeping_provider::Lookup_table::Instruction_entry
    Abstract_Housekeeping_provider::Lookup_table::instruction_table[] = {
    {&Abstract_Housekeeping_provider::p_DIAG, Cmd_DIAG},
    {&Abstract_Housekeeping_provider::p_HELP, Cmd_HELP},
    {&Abstract_Housekeeping_provider::p_STATUS, Cmd_STATUS},
    {&Abstract_Housekeeping_provider::p_TEST, Cmd_TEST},
    {&Abstract_Housekeeping_provider::p_VERSION, Cmd_VERSION},
};

namespace SpectraNode_interface{

    bool Abstract_Housekeeping_provider::on_indication(Instruction_major &transaction) {
        return Abstract_Housekeeping_provider::Lookup_table::lookup(this, transaction);
    }

    void Abstract_Housekeeping_provider::p_VERSION(Instruction_major &transaction){
        v_VERSION(transaction);
    }
            
    void Abstract_Housekeeping_provider::p_STATUS(Instruction_major &transaction){
        v_STATUS(transaction);
    }
            
    void Abstract_Housekeeping_provider::p_DIAG(Instruction_major &transaction){
        v_DIAG(transaction);
    }
            
    void Abstract_Housekeeping_provider::p_TEST(Instruction_major &transaction){
        v_TEST(transaction);
    }
            
    void Abstract_Housekeeping_provider::p_HELP(Instruction_major &transaction){
        v_HELP(transaction);
    }
            
}
