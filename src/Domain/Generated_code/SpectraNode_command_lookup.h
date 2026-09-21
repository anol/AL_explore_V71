

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

#ifndef SpectraNode_COMMAND_LOOKUP_h
#define SpectraNode_COMMAND_LOOKUP_h

#include <cstdint>

import Support.Instruction_token;

#include "SpectraNode_command_lookup.h"

namespace SpectraNode_interface{

    enum Command : uint16_t {
        No_such_command,
        Special_command,
        Cmd_DEMO_CADENCE_seconds,
        Cmd_DEMO_CHANNEL_channel,
        Cmd_IDLE_CADENCE_seconds,
        Cmd_MODE_DEMO,
        Cmd_MODE_IDLE,
        Cmd_MODE_NOMINAL,
        Cmd_NOMINAL_ALARM_micro_sievert,
        Cmd_NOMINAL_CADENCE_seconds,
        Cmd_FORMAT_CPS,
        Cmd_FORMAT_N42,
        Cmd_FORMAT_R6,
        Cmd_GET_RESET,
        Cmd_GET,
        Cmd_TIME_date_time,
        Cmd_CAL_ADC_V35_cal_35V,
        Cmd_CAL_ADC_V40_cal_40V,
        Cmd_CAL_DAC_V35_cal_35V,
        Cmd_CAL_DAC_V40_cal_40V,
        Cmd_CAL_DIAG,
        Cmd_CAL_TEST_BIAS,
        Cmd_CAL_TEST_NOISE,
        Cmd_CAL_TEST_OFFSET,
        Cmd_CAL_TEST_PEDESTAL,
        Cmd_CAL_TRACE,
        Cmd_ASIC_DUMP,
        Cmd_ASIC_LOAD,
        Cmd_ASIC_REG_reg_addr_data32,
        Cmd_ASIC_REG_reg_addr,
        Cmd_CONFIG_offset_data32,
        Cmd_CONFIG_offset,
        Cmd_CONFIG_APPLY,
        Cmd_CONFIG_CLEAN,
        Cmd_CONFIG_DUMP,
        Cmd_CONFIG_LOAD,
        Cmd_CONFIG_SAVE,
        Cmd_DIAG,
        Cmd_HELP,
        Cmd_STATUS,
        Cmd_TEST,
        Cmd_VERSION,
        
    };

    const Instruction::Instruction_token *get_commands();

}

#endif // SpectraNode_h
