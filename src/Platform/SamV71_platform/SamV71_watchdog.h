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
 */
/**
 * \date   IDEAS/06.05.2021/aeols
 * \brief
 */

#ifndef TARGET_SamV71_WATCHDOG_H
#define TARGET_SamV71_WATCHDOG_H


/// \brief Purpose: Watchdog encapsulation
/// \note The watchdogs are initialized in the bootloader reset ISR.
namespace SamV71 {
    namespace SamV71_watchdog {
        enum {
            Primary_watchdog_password = 0xA5,
            Reinforced_watchdog_password = 0xC4,
            // The watchdog clock is based on the slow clock (approx. 32KHz) divided by 128.
            // SamV71 Errata: "The slow RC frequency is 35 KHz instead of 32 KHz".
            // The primary watchdog has a max length period (approx. 15-16 seconds).
            Watchdog_counter = 0xFFF,
            Watchdog_delta = Watchdog_counter,
            // The reinforced watchdog clock is based on the Main RC oscillator (4MHz) and a divider,
            // giving a frequency assumed to be in the 32kHz area.
            Assumed_per_second = 32000 / 128,
            // The reinforced watchdog has a shorter period (approx 4 seconds).
            Timeout_seconds = 4,
            Reinforced_counter = Timeout_seconds * Assumed_per_second,
            Reinforced_fixed = 0xFFF,
        };

        static_assert(Reinforced_counter < (1 << 12), "Reinforced_counter out-of-range (12-bit)!");
        static_assert(Watchdog_counter < (1 << 12), "Reinforced_counter out-of-range (12-bit)!");

        using Watchdog_function = void (*)(void *);

        void set_watchdog_function(void *, Watchdog_function);

        bool was_reinforced_watchdog(bool clear);

        void check_watchdog();
    }
}

#endif //TARGET_SamV71_WATCHDOG_H
