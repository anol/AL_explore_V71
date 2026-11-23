

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

export module Domain.Keyword_lookup;

import Support.Token_type;

export namespace Domain_interface {

    constexpr int get_keyword_version() { return 5; }

    const char *get_keyword(unsigned char key);

    enum Keys : unsigned char {
        No_key,

        Key_a,
        Key_access,
        Key_active,
        Key_adc,
        Key_ADC,
        Key_ALARM,
        Key_APPLY,
        Key_ASIC,
        Key_asic_nbr,
        Key_b,
        Key_BIAS,
        Key_bias_mV,
        Key_c,
        Key_C25,
        Key_CADENCE,
        Key_CAL,
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
        Key_data12,
        Key_data16,
        Key_data32,
        Key_data8,
        Key_date,
        Key_demo,
        Key_DEMO,
        Key_device,
        Key_DIAG,
        Key_DISABLE,
        Key_DUMP,
        Key_ECHO,
        Key_ENABLE,
        Key_ERROR,
        Key_format,
        Key_FORMAT,
        Key_gain,
        Key_GET,
        Key_HELP,
        Key_high,
        Key_HIGH,
        Key_high_mV,
        Key_HOLD,
        Key_HOUSE,
        Key_Housekeeping,
        Key_idle,
        Key_IDLE,
        Key_INPUT,
        Key_Instrument_calibration,
        Key_integration_time,
        Key_LOAD,
        Key_low,
        Key_LOW,
        Key_low_mV,
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
        Key_OVERRIDE,
        Key_parameter,
        Key_pedestal,
        Key_PEDESTAL,
        Key_R6,
        Key_readout_count,
        Key_REG,
        Key_reg_addr,
        Key_RESET,
        Key_RESTORE,
        Key_result,
        Key_SAVE,
        Key_Science_data,
        Key_seconds,
        Key_serial_number,
        Key_SET,
        Key_simple_R6,
        Key_start_threshold,
        Key_STATUS,
        Key_stop_count,
        Key_temp,
        Key_TEMP,
        Key_TEST,
        Key_THRESHOLD,
        Key_time,
        Key_TIME,
        Key_TRACE,
        Key_v30,
        Key_v40,
        Key_VERSION,
        Key_VOLT,
        
        number_of_keys,
        Literal_value,
        Wildcard = Instruction::Wildcard_id
    };

}
