<!--



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


-->

# FW1038 SpectraNode command version 3

Date: 29.09.2026

## Provider: Mode_control

| Command | Description |
|---|---|
| `AT+DEMO_CADENCE=<seconds(int 0..1000000)>` | Set telemetry interval (s) in Demonstration Mode. |
| `AT+DEMO_CHANNEL=<channel(int 0..18)>` | Set channel to be used in live view demo. 0=inhibit readout, 1..16=input channel, 17=analog summing, 18=digital summing. |
| `AT+IDLE_CADENCE=<seconds(int 0..1000000)>` | Set telemetry interval (s) in Idle Mode. |
| `AT+MODE_DEMO` | Set demonstration mode. |
| `AT+MODE_IDLE` | Set idle mode. |
| `AT+MODE_NOMINAL` | Set nominal mode. |
| `AT+NOMINAL_ALARM=<micro_sievert(int 0..1000000)>` | Set alarm threshold (uSv/h). |
| `AT+NOMINAL_CADENCE=<seconds(int 0..1000000)>` | Set telemetry interval (s) in Nominal Mode. |

## Provider: Spectroscopic_data

| Command | Description |
|---|---|
| `AT+FORMAT_CPS` | Send event counters. |
| `AT+FORMAT_N42` | Use complex histogram format (N42). |
| `AT+FORMAT_R6` | Use simple histogram format (R6). |
| `AT+GET_RESET` | Send the histogram and clear the buffers. |
| `AT+GET` | Send the histogram. |
| `AT+TIME=<date(int 0..0x7FFFFFFF)>,<time(int 0..1000000)>` | Set date and time. |

## Provider: Instrument_calibration

| Command | Description |
|---|---|
| `AT+CAL_ADC_V35=<cal_35V(int -100000..-1)>` | Set bias ADC calibration 35V setpoint. |
| `AT+CAL_ADC_V40=<cal_40V(int -100000..-1)>` | Set bias ADC calibration 40V setpoint. |
| `AT+CAL_DAC_V35=<cal_35V(int -100000..-1)>` | Set bias DAC calibration 35V setpoint. |
| `AT+CAL_DAC_V40=<cal_40V(int -100000..-1)>` | Set bias DAC calibration 40V setpoint. |
| `AT+CAL_DIAG` | Misc. diagnostic information wrt. calibration. |
| `AT+CAL_TEST_BIAS` | Find common bias voltage using dark count rate. |
| `AT+CAL_TEST_NOISE` | Find channel noise floor using threshold scan. |
| `AT+CAL_TEST_OFFSET` | Find channel offset voltage using fixed source. |
| `AT+CAL_TEST_PEDESTAL` | Find channel pedestal using forced readout. |
| `AT+CAL_TRACE` | Toggle diagnostic trace on/off. |

## Provider: Configuration_manager

| Command | Description |
|---|---|
| `AT+ASIC=<asic_nbr(int 1..5)>` | Select ASIC 1-5. |
| `AT+ASIC_DUMP` | Dump all the ASIC registers. |
| `AT+ASIC_LOAD` | Update all ASIC registers from current configuration. |
| `AT+ASIC_REG=<reg_addr(int 0..32)>,<data32(int 0..0x7FFFFFFF)>` | Write the ASIC register. |
| `AT+ASIC_REG=<reg_addr(int 0..32)>` | Read the ASIC register. |
| `AT+ASIC` | Show current ASIC selection. |
| `AT+CONFIG_APPLY` | Apply the current configuration. |
| `AT+CONFIG_ASIC=<asic_nbr(int 1..5)>,<reg_addr(int 0..32)>,<data32(int 0..0x7FFFFFFF)>` | Set the current configuration register value. |
| `AT+CONFIG_ASIC=<asic_nbr(int 1..5)>,<reg_addr(int 0..32)>` | Get the current configuration register value. |
| `AT+CONFIG_CLEAN` | Clean the persistent storage and set the default configuration. |
| `AT+CONFIG_DUMP` | Dump the current configuration. |
| `AT+CONFIG_LOAD` | Load the current configuration from the persistent storage. |
| `AT+CONFIG=<offset(int 0..4095)>,<data32(int 0..0x7FFFFFFF)>` | Set configuration attribute value. |
| `AT+CONFIG=<offset(int 0..4095)>` | Get configuration attribute value. |
| `AT+CONFIG_SAVE` | Save the current configuration to the persistent storage. |

## Provider: Housekeeping

| Command | Description |
|---|---|
| `AT+DIAG` | Send miscellaneous diagnostic information. |
| `AT+HELP` | Show commands. |
| `AT+STATUS` | Send mode and other system data. |
| `AT+TEST` | Initiate a self-test sequence. |
| `AT+TRACE` | Toggle trace on/off. |
| `AT+VERSION` | Send version and other build information. |
