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

# SpectraNode_structure_definition

| Identifier | Type | Default | Description |
|---|---|---|---|
| device.serial_number | data32 | 0 | Device serial number. |
| device.reg_addr.[0-29] | data32 | 0x0200'0323 | IDE3380 ASIC register default (channel 1-16 control register template; see IDE3380_register_decoder for the rest). |
| MODE.active | idle\|nominal\|demo | Key_demo | Active operating mode at startup. |
| CADENCE.DEMO | seconds | 2 | Telemetry cadence in Demo mode. |
| CADENCE.NOMINAL | seconds | 2 | Telemetry cadence in Nominal mode. |
| CADENCE.IDLE | seconds | 2 | Telemetry cadence in Idle mode. |
| CHANNEL.DEMO | channel | 18 | Active channel in Demo mode, 0=inhibit, 1-16=input, 17=analog summing, 18=digital summing. |
| CHANNEL.NOMINAL | channel | 18 | Active channel in Nominal mode. |
| CHANNEL.IDLE | channel | 18 | Active channel in Idle mode. |
| format.DEMO | no_data\|legacy_live_view\|simple_R6\|only_CPS\|complex_N42\|housekeeping | Key_simple_R6 | Data format in Demo mode. |
| format.IDLE | no_data\|legacy_live_view\|simple_R6\|only_CPS\|complex_N42\|housekeeping | Key_simple_R6 | Data format in Idle mode. |
| format.NOMINAL | no_data\|legacy_live_view\|simple_R6\|only_CPS\|complex_N42\|housekeeping | Key_simple_R6 | Data format in Nominal mode. |
| calibration.parameter.a | data32 | 100000 | Calibration parameter A. |
| calibration.parameter.b | data32 | 10000 | Calibration parameter B. |
| calibration.BIAS | data32 | 0 | Bias voltage at 25 degC reference. |
| calibration.adc.V35 | mV | -35000 | ADC calibration setpoint at -35V. |
| calibration.adc.V40 | mV | -40000 | ADC calibration setpoint at -40V. |
| calibration.dac.V35 | mV | -35000 | DAC calibration setpoint at -35V. |
| calibration.dac.V40 | mV | -40000 | DAC calibration setpoint at -40V. |
| calibration.integration_time | number | 100 | Calibration pulse integration time. |
| calibration.start_threshold | number | 100 | Calibration start threshold. |
| calibration.stop_count | number | 100 | Calibration stop count. |
| calibration.readout_count | number | 100000 | Calibration readout count. |
| calibration.pedestal.[1-16] | data32 | 50 | Per-channel pedestal offset. |
| calibration.gain.[1-16] | data32 | 1 | Per-channel gain factor. |
