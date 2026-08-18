/*
 * Copyright (C) 2021 Integrated Detector Electronics AS
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
 * \date   IDEAS/11.02.2020/aeols
 * \brief
 */

#ifndef NORM_FW_CLOCK_INTERFACE_H
#define NORM_FW_CLOCK_INTERFACE_H

#include <cstdint>

#include <Utility/Utility_types.h>
#include <Transaction/Service_report.h>

/// Purpose: Hardware abstraction of the MCU clocks and timestamps.
namespace Clock_interface {
    extern void initialize_clocks(Frequency oscillator);

    extern void initialize_timers();

    extern uint32_t get_milliseconds();

    extern uint32_t get_master_clock();

    extern void report_state(Service_report *);

    extern uint32_t get_millisecond_timestamp();

    extern uint64_t get_microsecond_timestamp();

    extern uint32_t get_hundredthsecond_timestamp();
}


#endif //NORM_FW_CLOCK_INTERFACE_H
