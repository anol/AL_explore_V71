

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


export module Application.SpectraNode_command_help;

export namespace SpectraNode_interface{

    constexpr auto* help_DEMO_CADENCE_seconds{"Set telemetry interval (s) in Demonstration Mode."};
    constexpr auto* help_DEMO_CHANNEL_channel{"Set channel to be used in live view demo. 0=inhibit readout, 1..16=input channel, 17=analog summing, 18=digital summing."};
    constexpr auto* help_IDLE_CADENCE_seconds{"Set telemetry interval (s) in Idle Mode."};
    constexpr auto* help_MODE_DEMO{"Set demonstration mode."};
    constexpr auto* help_MODE_IDLE{"Set idle mode."};
    constexpr auto* help_MODE_NOMINAL{"Set nominal mode."};
    constexpr auto* help_NOMINAL_ALARM_micro_sievert{"Set alarm threshold (uSv/h)."};
    constexpr auto* help_NOMINAL_CADENCE_seconds{"Set telemetry interval (s) in Nominal Mode."};
    constexpr auto* help_FORMAT_CPS{"Send event counters."};
    constexpr auto* help_FORMAT_N42{"Use complex histogram format (N42)."};
    constexpr auto* help_FORMAT_R6{"Use simple histogram format (R6)."};
    constexpr auto* help_GET_RESET{"Send the histogram and clear the buffers."};
    constexpr auto* help_GET{"Send the histogram."};
    constexpr auto* help_TIME_date_time{"Set date and time."};
    constexpr auto* help_BIAS_ADC_SET_v30_v40{"Set the ADC -30V and -40V calibration points."};
    constexpr auto* help_BIAS_ADC_SET{"Get the ADC -30V and -40V calibration points."};
    constexpr auto* help_BIAS_ADC{"Get the raw ADC reading."};
    constexpr auto* help_BIAS_C25_bias_mV{"Set the 25C bias setpoint. 0=use temperature."};
    constexpr auto* help_BIAS_C25{"Get the 25C bias setpoint."};
    constexpr auto* help_BIAS_CAL{"Toggle calibration mode ON/OFF."};
    constexpr auto* help_BIAS_DAC_data12{"Override the DAC setting."};
    constexpr auto* help_BIAS_DAC_SET_v30_v40{"Set the DAC -30V and -40V calibration points."};
    constexpr auto* help_BIAS_DAC_SET{"Get the DAC -30V and -40V calibration points."};
    constexpr auto* help_BIAS_DAC{"Get the raw DAC setting."};
    constexpr auto* help_BIAS_DIAG{"Misc. diagnostic information wrt. bias voltage."};
    constexpr auto* help_BIAS_ERROR{"Get the bias voltage error."};
    constexpr auto* help_BIAS_SET_bias_mV{"Override the temp. compensated bias setpoint."};
    constexpr auto* help_BIAS_SET{"Get the temp. compensated bias setpoint."};
    constexpr auto* help_BIAS_TEMP_SET_a_b_c{"Set the temperature correction factors."};
    constexpr auto* help_BIAS_TEMP_SET{"Get the temperature correction."};
    constexpr auto* help_BIAS_TEMP{"Get the temperature."};
    constexpr auto* help_BIAS_VOLT{"Get the bias voltage."};
    constexpr auto* help_BIAS{"Show setpoint, voltage and error."};
    constexpr auto* help_CAL_DIAG{"Misc. diagnostic information wrt. calibration."};
    constexpr auto* help_CAL_DUMP{"Get calibration settings."};
    constexpr auto* help_CAL_TEST_BIAS{"Find common bias voltage using dark count rate."};
    constexpr auto* help_CAL_TEST_NOISE{"Find channel noise floor using threshold scan."};
    constexpr auto* help_CAL_TEST_OFFSET{"Find channel offset voltage using fixed source."};
    constexpr auto* help_CAL_TEST_PEDESTAL{"Find channel pedestal using forced readout."};
    constexpr auto* help_CAL_TRACE{"Toggle diagnostic trace on/off."};
    constexpr auto* help_ASIC_asic_nbr{"Select ASIC 1-5."};
    constexpr auto* help_ASIC_DUMP{"Dump all the ASIC registers."};
    constexpr auto* help_ASIC_LOAD{"Update all ASIC registers from current configuration."};
    constexpr auto* help_ASIC_REG_reg_addr_data32{"Write the ASIC register."};
    constexpr auto* help_ASIC_REG_reg_addr{"Read the ASIC register."};
    constexpr auto* help_ASIC{"Show current ASIC selection."};
    constexpr auto* help_CONFIG_offset_data32{"Set configuration attribute value."};
    constexpr auto* help_CONFIG_offset{"Get configuration attribute value."};
    constexpr auto* help_CONFIG_APPLY{"Apply the current configuration."};
    constexpr auto* help_CONFIG_ASIC_asic_nbr_reg_addr_data32{"Set the current configuration register value."};
    constexpr auto* help_CONFIG_ASIC_asic_nbr_reg_addr{"Get the current configuration register value."};
    constexpr auto* help_CONFIG_CLEAN{"Clean the persistent storage and set the default configuration."};
    constexpr auto* help_CONFIG_DUMP{"Dump the current configuration."};
    constexpr auto* help_CONFIG_LOAD{"Load the current configuration from the persistent storage."};
    constexpr auto* help_CONFIG_SAVE{"Save the current configuration to the persistent storage."};
    constexpr auto* help_DIAG{"Send miscellaneous diagnostic information."};
    constexpr auto* help_ECHO{"Toggle console echo on/off."};
    constexpr auto* help_HELP{"Show commands."};
    constexpr auto* help_STATUS{"Send mode and other system data."};
    constexpr auto* help_TEST{"Initiate a self-test sequence."};
    constexpr auto* help_TRACE{"Toggle trace on/off."};
    constexpr auto* help_VERSION{"Send version and other build information."};
    
}
