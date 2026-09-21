

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


#pragma once

#include "Instruction_lookup.h"
import Type.Abstract_provider;

using namespace Abstract;

namespace Transaction {
    class Transaction_major;
}

using namespace Transaction;

namespace SpectraNode_interface {
    
    class Abstract_Mode_control_provider : public Abstract_provider<Instruction_major> {
    public:
       using Lookup_table = Instruction_lookup<Abstract_Mode_control_provider>;
    public:
        Abstract_Mode_control_provider() : Abstract_provider(Key_Mode_control){}
        bool on_indication(Instruction_major &) override;
    protected:
        virtual void v_MODE_DEMO(Instruction_major &) = 0;
        virtual void v_MODE_IDLE(Instruction_major &) = 0;
        virtual void v_MODE_NOMINAL(Instruction_major &) = 0;
        virtual void v_IDLE_CADENCE_seconds(Instruction_major &, int seconds_2) = 0;
        virtual void v_DEMO_CADENCE_seconds(Instruction_major &, int seconds_2) = 0;
        virtual void v_DEMO_CHANNEL_channel(Instruction_major &, int channel_2) = 0;
        virtual void v_NOMINAL_ALARM_micro_sievert(Instruction_major &, int micro_sievert_2) = 0;
        virtual void v_NOMINAL_CADENCE_seconds(Instruction_major &, int seconds_2) = 0;
    public:
        void p_DEMO_CADENCE_seconds(Instruction_major &);
        void p_DEMO_CHANNEL_channel(Instruction_major &);
        void p_IDLE_CADENCE_seconds(Instruction_major &);
        void p_MODE_DEMO(Instruction_major &);
        void p_MODE_IDLE(Instruction_major &);
        void p_MODE_NOMINAL(Instruction_major &);
        void p_NOMINAL_ALARM_micro_sievert(Instruction_major &);
        void p_NOMINAL_CADENCE_seconds(Instruction_major &);
    };
            
    class Abstract_Spectroscopic_data_provider : public Abstract_provider<Instruction_major> {
    public:
       using Lookup_table = Instruction_lookup<Abstract_Spectroscopic_data_provider>;
    public:
        Abstract_Spectroscopic_data_provider() : Abstract_provider(Key_Spectroscopic_data){}
        bool on_indication(Instruction_major &) override;
    protected:
        virtual void v_GET_RESET(Instruction_major &) = 0;
        virtual void v_GET(Instruction_major &) = 0;
        virtual void v_FORMAT_CPS(Instruction_major &) = 0;
        virtual void v_FORMAT_N42(Instruction_major &) = 0;
        virtual void v_FORMAT_R6(Instruction_major &) = 0;
        virtual void v_TIME_date_time(Instruction_major &, int date_1, int time_2) = 0;
    public:
        void p_FORMAT_CPS(Instruction_major &);
        void p_FORMAT_N42(Instruction_major &);
        void p_FORMAT_R6(Instruction_major &);
        void p_GET_RESET(Instruction_major &);
        void p_GET(Instruction_major &);
        void p_TIME_date_time(Instruction_major &);
    };
            
    class Abstract_Instrument_calibration_provider : public Abstract_provider<Instruction_major> {
    public:
       using Lookup_table = Instruction_lookup<Abstract_Instrument_calibration_provider>;
    public:
        Abstract_Instrument_calibration_provider() : Abstract_provider(Key_Instrument_calibration){}
        bool on_indication(Instruction_major &) override;
    protected:
        virtual void v_CAL_ADC_V35_cal_35V(Instruction_major &, int cal_35V_3) = 0;
        virtual void v_CAL_ADC_V40_cal_40V(Instruction_major &, int cal_40V_3) = 0;
        virtual void v_CAL_DAC_V35_cal_35V(Instruction_major &, int cal_35V_3) = 0;
        virtual void v_CAL_DAC_V40_cal_40V(Instruction_major &, int cal_40V_3) = 0;
        virtual void v_CAL_DIAG(Instruction_major &) = 0;
        virtual void v_CAL_TEST_BIAS(Instruction_major &) = 0;
        virtual void v_CAL_TEST_NOISE(Instruction_major &) = 0;
        virtual void v_CAL_TEST_OFFSET(Instruction_major &) = 0;
        virtual void v_CAL_TEST_PEDESTAL(Instruction_major &) = 0;
        virtual void v_CAL_TRACE(Instruction_major &) = 0;
    public:
        void p_CAL_ADC_V35_cal_35V(Instruction_major &);
        void p_CAL_ADC_V40_cal_40V(Instruction_major &);
        void p_CAL_DAC_V35_cal_35V(Instruction_major &);
        void p_CAL_DAC_V40_cal_40V(Instruction_major &);
        void p_CAL_DIAG(Instruction_major &);
        void p_CAL_TEST_BIAS(Instruction_major &);
        void p_CAL_TEST_NOISE(Instruction_major &);
        void p_CAL_TEST_OFFSET(Instruction_major &);
        void p_CAL_TEST_PEDESTAL(Instruction_major &);
        void p_CAL_TRACE(Instruction_major &);
    };
            
    class Abstract_Configuration_manager_provider : public Abstract_provider<Instruction_major> {
    public:
       using Lookup_table = Instruction_lookup<Abstract_Configuration_manager_provider>;
    public:
        Abstract_Configuration_manager_provider() : Abstract_provider(Key_Configuration_manager){}
        bool on_indication(Instruction_major &) override;
    protected:
        virtual void v_CONFIG_offset_data32(Instruction_major &, int offset_1, int data32_2) = 0;
        virtual void v_CONFIG_offset(Instruction_major &, int offset_1) = 0;
        virtual void v_CONFIG_APPLY(Instruction_major &) = 0;
        virtual void v_CONFIG_CLEAN(Instruction_major &) = 0;
        virtual void v_CONFIG_DUMP(Instruction_major &) = 0;
        virtual void v_CONFIG_LOAD(Instruction_major &) = 0;
        virtual void v_CONFIG_SAVE(Instruction_major &) = 0;
        virtual void v_ASIC_DUMP(Instruction_major &) = 0;
        virtual void v_ASIC_LOAD(Instruction_major &) = 0;
        virtual void v_ASIC_REG_reg_addr_data32(Instruction_major &, int reg_addr_2, int data32_3) = 0;
        virtual void v_ASIC_REG_reg_addr(Instruction_major &, int reg_addr_2) = 0;
    public:
        void p_ASIC_DUMP(Instruction_major &);
        void p_ASIC_LOAD(Instruction_major &);
        void p_ASIC_REG_reg_addr_data32(Instruction_major &);
        void p_ASIC_REG_reg_addr(Instruction_major &);
        void p_CONFIG_offset_data32(Instruction_major &);
        void p_CONFIG_offset(Instruction_major &);
        void p_CONFIG_APPLY(Instruction_major &);
        void p_CONFIG_CLEAN(Instruction_major &);
        void p_CONFIG_DUMP(Instruction_major &);
        void p_CONFIG_LOAD(Instruction_major &);
        void p_CONFIG_SAVE(Instruction_major &);
    };
            
    class Abstract_Housekeeping_provider : public Abstract_provider<Instruction_major> {
    public:
       using Lookup_table = Instruction_lookup<Abstract_Housekeeping_provider>;
    public:
        Abstract_Housekeeping_provider() : Abstract_provider(Key_Housekeeping){}
        bool on_indication(Instruction_major &) override;
    protected:
        virtual void v_VERSION(Instruction_major &) = 0;
        virtual void v_STATUS(Instruction_major &) = 0;
        virtual void v_DIAG(Instruction_major &) = 0;
        virtual void v_TEST(Instruction_major &) = 0;
        virtual void v_HELP(Instruction_major &) = 0;
    public:
        void p_DIAG(Instruction_major &);
        void p_HELP(Instruction_major &);
        void p_STATUS(Instruction_major &);
        void p_TEST(Instruction_major &);
        void p_VERSION(Instruction_major &);
    };
            

}

