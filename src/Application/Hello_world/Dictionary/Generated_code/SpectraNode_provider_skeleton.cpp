

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
#include <iostream>

module Application.SpectraNode_provider_skeleton;

using namespace SpectraNode_interface;
        
void Implement_Mode_control_provider::v_DEMO_CADENCE_seconds(Transaction_major &, int seconds_2){
    std::cout << "Implement_Mode_control_provider::v_DEMO_CADENCE_seconds:" << "seconds_2=" << seconds_2 << std::endl;
}
            
void Implement_Mode_control_provider::v_DEMO_CHANNEL_channel(Transaction_major &, int channel_2){
    std::cout << "Implement_Mode_control_provider::v_DEMO_CHANNEL_channel:" << "channel_2=" << channel_2 << std::endl;
}
            
void Implement_Mode_control_provider::v_IDLE_CADENCE_seconds(Transaction_major &, int seconds_2){
    std::cout << "Implement_Mode_control_provider::v_IDLE_CADENCE_seconds:" << "seconds_2=" << seconds_2 << std::endl;
}
            
void Implement_Mode_control_provider::v_MODE_DEMO(Transaction_major &){
    std::cout << "Implement_Mode_control_provider::v_MODE_DEMO:" << std::endl;
}
            
void Implement_Mode_control_provider::v_MODE_IDLE(Transaction_major &){
    std::cout << "Implement_Mode_control_provider::v_MODE_IDLE:" << std::endl;
}
            
void Implement_Mode_control_provider::v_MODE_NOMINAL(Transaction_major &){
    std::cout << "Implement_Mode_control_provider::v_MODE_NOMINAL:" << std::endl;
}
            
void Implement_Mode_control_provider::v_NOMINAL_ALARM_micro_sievert(Transaction_major &, int micro_sievert_2){
    std::cout << "Implement_Mode_control_provider::v_NOMINAL_ALARM_micro_sievert:" << "micro_sievert_2=" << micro_sievert_2 << std::endl;
}
            
void Implement_Mode_control_provider::v_NOMINAL_CADENCE_seconds(Transaction_major &, int seconds_2){
    std::cout << "Implement_Mode_control_provider::v_NOMINAL_CADENCE_seconds:" << "seconds_2=" << seconds_2 << std::endl;
}
            
void Implement_Spectroscopic_data_provider::v_FORMAT_CPS(Transaction_major &){
    std::cout << "Implement_Spectroscopic_data_provider::v_FORMAT_CPS:" << std::endl;
}
            
void Implement_Spectroscopic_data_provider::v_FORMAT_N42(Transaction_major &){
    std::cout << "Implement_Spectroscopic_data_provider::v_FORMAT_N42:" << std::endl;
}
            
void Implement_Spectroscopic_data_provider::v_FORMAT_R6(Transaction_major &){
    std::cout << "Implement_Spectroscopic_data_provider::v_FORMAT_R6:" << std::endl;
}
            
void Implement_Spectroscopic_data_provider::v_GET_RESET(Transaction_major &){
    std::cout << "Implement_Spectroscopic_data_provider::v_GET_RESET:" << std::endl;
}
            
void Implement_Spectroscopic_data_provider::v_GET(Transaction_major &){
    std::cout << "Implement_Spectroscopic_data_provider::v_GET:" << std::endl;
}
            
void Implement_Spectroscopic_data_provider::v_TIME_date_time(Transaction_major &, int date_1, int time_2){
    std::cout << "Implement_Spectroscopic_data_provider::v_TIME_date_time:" << "date_1=" << date_1 << ",time_2=" << time_2 << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_BIAS_bias_mV(Transaction_major &, int bias_mV_1){
    std::cout << "Implement_Instrument_calibration_provider::v_BIAS_bias_mV:" << "bias_mV_1=" << bias_mV_1 << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_BIAS_ADC_V35_cal_35V(Transaction_major &, int cal_35V_3){
    std::cout << "Implement_Instrument_calibration_provider::v_BIAS_ADC_V35_cal_35V:" << "cal_35V_3=" << cal_35V_3 << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_BIAS_ADC_V40_cal_40V(Transaction_major &, int cal_40V_3){
    std::cout << "Implement_Instrument_calibration_provider::v_BIAS_ADC_V40_cal_40V:" << "cal_40V_3=" << cal_40V_3 << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_BIAS_ADC(Transaction_major &){
    std::cout << "Implement_Instrument_calibration_provider::v_BIAS_ADC:" << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_BIAS_DAC_data16(Transaction_major &, int data16_2){
    std::cout << "Implement_Instrument_calibration_provider::v_BIAS_DAC_data16:" << "data16_2=" << data16_2 << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_BIAS_DAC_V35_cal_35V(Transaction_major &, int cal_35V_3){
    std::cout << "Implement_Instrument_calibration_provider::v_BIAS_DAC_V35_cal_35V:" << "cal_35V_3=" << cal_35V_3 << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_BIAS_DAC_V40_cal_40V(Transaction_major &, int cal_40V_3){
    std::cout << "Implement_Instrument_calibration_provider::v_BIAS_DAC_V40_cal_40V:" << "cal_40V_3=" << cal_40V_3 << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_BIAS_DAC(Transaction_major &){
    std::cout << "Implement_Instrument_calibration_provider::v_BIAS_DAC:" << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_BIAS_DIAG(Transaction_major &){
    std::cout << "Implement_Instrument_calibration_provider::v_BIAS_DIAG:" << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_BIAS_VOLT(Transaction_major &){
    std::cout << "Implement_Instrument_calibration_provider::v_BIAS_VOLT:" << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_BIAS(Transaction_major &){
    std::cout << "Implement_Instrument_calibration_provider::v_BIAS:" << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_CAL_DIAG(Transaction_major &){
    std::cout << "Implement_Instrument_calibration_provider::v_CAL_DIAG:" << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_CAL_DUMP(Transaction_major &){
    std::cout << "Implement_Instrument_calibration_provider::v_CAL_DUMP:" << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_CAL_TEST_BIAS(Transaction_major &){
    std::cout << "Implement_Instrument_calibration_provider::v_CAL_TEST_BIAS:" << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_CAL_TEST_NOISE(Transaction_major &){
    std::cout << "Implement_Instrument_calibration_provider::v_CAL_TEST_NOISE:" << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_CAL_TEST_OFFSET(Transaction_major &){
    std::cout << "Implement_Instrument_calibration_provider::v_CAL_TEST_OFFSET:" << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_CAL_TEST_PEDESTAL(Transaction_major &){
    std::cout << "Implement_Instrument_calibration_provider::v_CAL_TEST_PEDESTAL:" << std::endl;
}
            
void Implement_Instrument_calibration_provider::v_CAL_TRACE(Transaction_major &){
    std::cout << "Implement_Instrument_calibration_provider::v_CAL_TRACE:" << std::endl;
}
            
void Implement_Configuration_manager_provider::v_ASIC_asic_nbr(Transaction_major &, int asic_nbr_1){
    std::cout << "Implement_Configuration_manager_provider::v_ASIC_asic_nbr:" << "asic_nbr_1=" << asic_nbr_1 << std::endl;
}
            
void Implement_Configuration_manager_provider::v_ASIC_DUMP(Transaction_major &){
    std::cout << "Implement_Configuration_manager_provider::v_ASIC_DUMP:" << std::endl;
}
            
void Implement_Configuration_manager_provider::v_ASIC_LOAD(Transaction_major &){
    std::cout << "Implement_Configuration_manager_provider::v_ASIC_LOAD:" << std::endl;
}
            
void Implement_Configuration_manager_provider::v_ASIC_REG_reg_addr_data32(Transaction_major &, int reg_addr_2, int data32_3){
    std::cout << "Implement_Configuration_manager_provider::v_ASIC_REG_reg_addr_data32:" << "reg_addr_2=" << reg_addr_2 << ",data32_3=" << data32_3 << std::endl;
}
            
void Implement_Configuration_manager_provider::v_ASIC_REG_reg_addr(Transaction_major &, int reg_addr_2){
    std::cout << "Implement_Configuration_manager_provider::v_ASIC_REG_reg_addr:" << "reg_addr_2=" << reg_addr_2 << std::endl;
}
            
void Implement_Configuration_manager_provider::v_ASIC(Transaction_major &){
    std::cout << "Implement_Configuration_manager_provider::v_ASIC:" << std::endl;
}
            
void Implement_Configuration_manager_provider::v_CONFIG_offset_data32(Transaction_major &, int offset_1, int data32_2){
    std::cout << "Implement_Configuration_manager_provider::v_CONFIG_offset_data32:" << "offset_1=" << offset_1 << ",data32_2=" << data32_2 << std::endl;
}
            
void Implement_Configuration_manager_provider::v_CONFIG_offset(Transaction_major &, int offset_1){
    std::cout << "Implement_Configuration_manager_provider::v_CONFIG_offset:" << "offset_1=" << offset_1 << std::endl;
}
            
void Implement_Configuration_manager_provider::v_CONFIG_APPLY(Transaction_major &){
    std::cout << "Implement_Configuration_manager_provider::v_CONFIG_APPLY:" << std::endl;
}
            
void Implement_Configuration_manager_provider::v_CONFIG_ASIC_asic_nbr_reg_addr_data32(Transaction_major &, int asic_nbr_2, int reg_addr_3, int data32_4){
    std::cout << "Implement_Configuration_manager_provider::v_CONFIG_ASIC_asic_nbr_reg_addr_data32:" << "asic_nbr_2=" << asic_nbr_2 << ",reg_addr_3=" << reg_addr_3 << ",data32_4=" << data32_4 << std::endl;
}
            
void Implement_Configuration_manager_provider::v_CONFIG_ASIC_asic_nbr_reg_addr(Transaction_major &, int asic_nbr_2, int reg_addr_3){
    std::cout << "Implement_Configuration_manager_provider::v_CONFIG_ASIC_asic_nbr_reg_addr:" << "asic_nbr_2=" << asic_nbr_2 << ",reg_addr_3=" << reg_addr_3 << std::endl;
}
            
void Implement_Configuration_manager_provider::v_CONFIG_CLEAN(Transaction_major &){
    std::cout << "Implement_Configuration_manager_provider::v_CONFIG_CLEAN:" << std::endl;
}
            
void Implement_Configuration_manager_provider::v_CONFIG_DUMP(Transaction_major &){
    std::cout << "Implement_Configuration_manager_provider::v_CONFIG_DUMP:" << std::endl;
}
            
void Implement_Configuration_manager_provider::v_CONFIG_LOAD(Transaction_major &){
    std::cout << "Implement_Configuration_manager_provider::v_CONFIG_LOAD:" << std::endl;
}
            
void Implement_Configuration_manager_provider::v_CONFIG_SAVE(Transaction_major &){
    std::cout << "Implement_Configuration_manager_provider::v_CONFIG_SAVE:" << std::endl;
}
            
void Implement_Housekeeping_provider::v_DIAG(Transaction_major &){
    std::cout << "Implement_Housekeeping_provider::v_DIAG:" << std::endl;
}
            
void Implement_Housekeeping_provider::v_ECHO(Transaction_major &){
    std::cout << "Implement_Housekeeping_provider::v_ECHO:" << std::endl;
}
            
void Implement_Housekeeping_provider::v_HELP(Transaction_major &){
    std::cout << "Implement_Housekeeping_provider::v_HELP:" << std::endl;
}
            
void Implement_Housekeeping_provider::v_STATUS(Transaction_major &){
    std::cout << "Implement_Housekeeping_provider::v_STATUS:" << std::endl;
}
            
void Implement_Housekeeping_provider::v_TEST(Transaction_major &){
    std::cout << "Implement_Housekeeping_provider::v_TEST:" << std::endl;
}
            
void Implement_Housekeeping_provider::v_TRACE(Transaction_major &){
    std::cout << "Implement_Housekeeping_provider::v_TRACE:" << std::endl;
}
            
void Implement_Housekeeping_provider::v_VERSION(Transaction_major &){
    std::cout << "Implement_Housekeeping_provider::v_VERSION:" << std::endl;
}
            
