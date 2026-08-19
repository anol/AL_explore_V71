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

#include "sam.h"

#include "SamV71_watchdog.h"


namespace SamV71
{
    namespace SamV71_watchdog
    {
        Watchdog_function optional_watchdog_func{};
        void* optional_watchdog_user{};
    }
}

namespace Interrupt_service_routines
{
    __attribute__ ((section(".ISR"))) void ISR_primary_watchdog()
    {
        // Device_interface::hardware_reset();
    }
}

namespace SamV71
{
    namespace SamV71_watchdog
    {
        void set_watchdog_function(void* user, Watchdog_function func)
        {
            optional_watchdog_user = user;
            optional_watchdog_func = func;
            __DSB();
            __ISB();
            WDT_REGS->WDT_MR =
                WDT_MR_WDFIEN(1) |
                WDT_MR_WDD(Reinforced_fixed) | WDT_MR_WDV(Reinforced_counter);
            __DSB();
            __ISB(); // Enable the Reinforced Safety Watchdog Timer interrupt in the NVIC
            enum : uint32_t { RSWDT_interrupt = WDT_IRQn };
            NVIC->ISER[RSWDT_interrupt >> 5] = 1 << (RSWDT_interrupt & 0x1F);
        }

        void check_watchdog()
        {
            __DSB();
            __ISB();
            // Reset the primary watchdog.
            WDT_REGS->WDT_CR = WDT_CR_WDRSTT(1) | WDT_CR_KEY(Primary_watchdog_password);
            __DSB();
            __ISB();
        }
    } // SamV71_watchdog
} // SamV71
