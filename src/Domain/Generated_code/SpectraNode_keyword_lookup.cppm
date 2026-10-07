

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

export module Domain.SpectraNode_keyword_lookup;
import Support.Token_type;

export namespace SpectraNode_interface {

    constexpr int get_keyword_version() { return 2; }

    const char *get_keyword(unsigned char key);

    enum Keys : unsigned char {
        No_key,

        Key_a,
        Key_active,
        Key_adc,
        Key_ADC,
        Key_ALARM,
        Key_APPLY,
        Key_ASIC,
        Key_asic_nbr,
        Key_b,
        Key_bias,
        Key_BIAS,
        Key_CADENCE,
        Key_CAL,
        Key_cal_35V,
        Key_cal_40V,
        Key_calibration,
        Key_channel,
        Key_CHANNEL,
        Key_CLEAN,
        Key_CONFIG,
        Key_Configuration_manager,
        Key_CPS,
        Key_CSV,
        Key_dac,
        Key_DAC,
        Key_data16,
        Key_data32,
        Key_data8,
        Key_date,
        Key_demo,
        Key_DEMO,
        Key_device,
        Key_DIAG,
        Key_DUMP,
        Key_ECHO,
        Key_format,
        Key_FORMAT,
        Key_gain,
        Key_GET,
        Key_HELP,
        Key_Housekeeping,
        Key_idle,
        Key_IDLE,
        Key_INPUT,
        Key_Instrument_calibration,
        Key_integration_time,
        Key_LOAD,
        Key_micro_sievert,
        Key_millis,
        Key_MODE,
        Key_Mode_control,
        Key_N42,
        Key_NOISE,
        Key_nominal,
        Key_NOMINAL,
        Key_offset,
        Key_OFFSET,
        Key_parameter,
        Key_pedestal,
        Key_PEDESTAL,
        Key_R6,
        Key_readout_count,
        Key_REG,
        Key_reg_addr,
        Key_RESET,
        Key_SAVE,
        Key_seconds,
        Key_serial_number,
        Key_simple_R6,
        Key_Spectroscopic_data,
        Key_start_threshold,
        Key_STATUS,
        Key_stop_count,
        Key_TEST,
        Key_time,
        Key_TIME,
        Key_TRACE,
        Key_V35,
        Key_V40,
        Key_VBIAS,
        Key_VERSION,
        
        number_of_keys,
        Literal_value,
        Wildcard = Instruction::Wildcard_id
    };

}
