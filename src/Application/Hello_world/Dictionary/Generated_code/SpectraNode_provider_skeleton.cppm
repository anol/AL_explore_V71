

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

export module Application.SpectraNode_provider_skeleton;
import Application.SpectraNode_provider_indication;

export namespace SpectraNode_interface {
    
    class Implement_Mode_control_provider : public Abstract_Mode_control_provider{
    protected:
        void v_MODE_DEMO(Transaction_major &) override;
        void v_MODE_IDLE(Transaction_major &) override;
        void v_MODE_NOMINAL(Transaction_major &) override;
        void v_IDLE_CADENCE_seconds(Transaction_major &, int seconds_2) override;
        void v_DEMO_CADENCE_seconds(Transaction_major &, int seconds_2) override;
        void v_DEMO_CHANNEL_channel(Transaction_major &, int channel_2) override;
        void v_NOMINAL_ALARM_micro_sievert(Transaction_major &, int micro_sievert_2) override;
        void v_NOMINAL_CADENCE_seconds(Transaction_major &, int seconds_2) override;
    };
            
    class Implement_Spectroscopic_data_provider : public Abstract_Spectroscopic_data_provider{
    protected:
        void v_GET_RESET(Transaction_major &) override;
        void v_GET(Transaction_major &) override;
        void v_FORMAT_CPS(Transaction_major &) override;
        void v_FORMAT_N42(Transaction_major &) override;
        void v_FORMAT_R6(Transaction_major &) override;
        void v_TIME_date_time(Transaction_major &, int date_1, int time_2) override;
    };
            
    class Implement_Instrument_calibration_provider : public Abstract_Instrument_calibration_provider{
    protected:
        void v_BIAS_bias_mV(Transaction_major &, int bias_mV_1) override;
        void v_BIAS_ADC_V35_cal_35V(Transaction_major &, int cal_35V_3) override;
        void v_BIAS_ADC_V40_cal_40V(Transaction_major &, int cal_40V_3) override;
        void v_BIAS_ADC(Transaction_major &) override;
        void v_BIAS_DAC_data16(Transaction_major &, int data16_2) override;
        void v_BIAS_DAC_V35_cal_35V(Transaction_major &, int cal_35V_3) override;
        void v_BIAS_DAC_V40_cal_40V(Transaction_major &, int cal_40V_3) override;
        void v_BIAS_DAC(Transaction_major &) override;
        void v_BIAS_DIAG(Transaction_major &) override;
        void v_BIAS_VOLT(Transaction_major &) override;
        void v_BIAS(Transaction_major &) override;
        void v_CAL_DIAG(Transaction_major &) override;
        void v_CAL_DUMP(Transaction_major &) override;
        void v_CAL_TEST_BIAS(Transaction_major &) override;
        void v_CAL_TEST_NOISE(Transaction_major &) override;
        void v_CAL_TEST_OFFSET(Transaction_major &) override;
        void v_CAL_TEST_PEDESTAL(Transaction_major &) override;
        void v_CAL_TRACE(Transaction_major &) override;
    };
            
    class Implement_Configuration_manager_provider : public Abstract_Configuration_manager_provider{
    protected:
        void v_CONFIG_offset_data32(Transaction_major &, int offset_1, int data32_2) override;
        void v_CONFIG_offset(Transaction_major &, int offset_1) override;
        void v_CONFIG_APPLY(Transaction_major &) override;
        void v_CONFIG_ASIC_asic_nbr_reg_addr_data32(Transaction_major &, int asic_nbr_2, int reg_addr_3, int data32_4) override;
        void v_CONFIG_ASIC_asic_nbr_reg_addr(Transaction_major &, int asic_nbr_2, int reg_addr_3) override;
        void v_CONFIG_CLEAN(Transaction_major &) override;
        void v_CONFIG_DUMP(Transaction_major &) override;
        void v_CONFIG_LOAD(Transaction_major &) override;
        void v_CONFIG_SAVE(Transaction_major &) override;
        void v_ASIC_asic_nbr(Transaction_major &, int asic_nbr_1) override;
        void v_ASIC_DUMP(Transaction_major &) override;
        void v_ASIC_LOAD(Transaction_major &) override;
        void v_ASIC_REG_reg_addr_data32(Transaction_major &, int reg_addr_2, int data32_3) override;
        void v_ASIC_REG_reg_addr(Transaction_major &, int reg_addr_2) override;
        void v_ASIC(Transaction_major &) override;
    };
            
    class Implement_Housekeeping_provider : public Abstract_Housekeeping_provider{
    protected:
        void v_DIAG(Transaction_major &) override;
        void v_ECHO(Transaction_major &) override;
        void v_HELP(Transaction_major &) override;
        void v_STATUS(Transaction_major &) override;
        void v_TEST(Transaction_major &) override;
        void v_TRACE(Transaction_major &) override;
        void v_VERSION(Transaction_major &) override;
    };
            
}
