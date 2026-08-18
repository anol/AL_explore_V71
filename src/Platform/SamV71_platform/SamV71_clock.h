/*
 * Copyright (C) 2024 Integrated Detector Electronics AS
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

/**
 * \date   IDEAS/31.01.2020/aeols
 * \brief
 */

#ifndef NORM_FW_SamV71_CLOCK_H
#define NORM_FW_SamV71_CLOCK_H


#include "Interface/Clock_interface.h"
#include "Transaction/Service_report.h"

namespace Clock_interface {
    extern Frequency the_master_Hz;
    extern Frequency the_PLLA_Hz;
    extern Frequency the_PLLB_Hz;
    extern volatile uint32_t channel_status;
    extern volatile uint32_t milliseconds_allmost_since_start;
    extern volatile uint32_t hundredthseconds_timestamp;

    extern uint32_t enable_peripheral_clock(uint32_t peripheral_id);

    extern uint32_t get_channel_status();

    extern uint32_t get_microsecond_clock();

    extern void setup_millisecond_timer();

    extern void setup_microsecond_timer();

    extern void setup_hundredthsecond_timer();

    extern void disable_PLLs();

    extern void initialize_main_clock();

    extern Frequency initialize_PLLA(Frequency oscillator);

    extern Frequency initialize_PLLB(Frequency oscillator);

    extern Frequency initialize_master_clock(Frequency PLLA);
}


#endif //NORM_FW_SamV71_CLOCK_H
