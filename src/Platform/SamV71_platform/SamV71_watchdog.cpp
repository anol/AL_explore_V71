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

#include "samrh71f20c.h"
#include "Interface/Device_interface.h"

#include "SamV71_watchdog.h"

#include <cortex_m7_instructions.h>

#include "Bookkeeping/NORM_bookkeeping.h"
#include "Bookkeeping/Persistent_watchdog_info.h"
#include "component/memory/Cache.h"


namespace SamV71 {
    namespace SamV71_watchdog {
        Watchdog_function optional_watchdog_func{};
        void *optional_watchdog_user{};
    }
}

namespace Interrupt_service_routines {
    __attribute__ ((section(".ISR"))) void ISR_primary_watchdog() {
        Device_interface::hardware_reset();
    }

    __attribute__ ((section(".ISR"))) void ISR_reinforced_watchdog() {
        using namespace Bookkeeping;
        using namespace SamV71::SamV71_watchdog;
        auto &wd_info = NORM_bookkeeping::get_persistent_watchdog_info();

        wd_info.the_event_flag = Persistent_watchdog_info::Event_flag;
        wd_info.the_event_count++;

        if (optional_watchdog_func) {
            optional_watchdog_func(optional_watchdog_user);
            __DSB();
            __ISB();
        }
        Cache::CleanDCache();
        Device_interface::hardware_reset();
    }
}

namespace SamV71 {
    namespace SamV71_watchdog {
        bool was_reinforced_watchdog(bool clear) {
            using namespace Bookkeeping;
            auto &wd_info = NORM_bookkeeping::get_persistent_watchdog_info();

            auto result  = (Persistent_watchdog_info::Event_flag == wd_info.the_event_flag);
            if (clear)
            {
                wd_info.the_event_flag = Persistent_watchdog_info::No_event;
            }

            return result;
        }

        void set_watchdog_function(void *user, Watchdog_function func) {
            using namespace Bookkeeping;
            auto &wd_info = NORM_bookkeeping::get_persistent_watchdog_info();

            // Clear the watchdog info
            if ((wd_info.the_sentinel_1 != Persistent_watchdog_info::Sentinel_1) ||
                (wd_info.the_sentinel_2 != Persistent_watchdog_info::Sentinel_2)) {
                wd_info.the_sentinel_1 = Persistent_watchdog_info::Sentinel_1;
                wd_info.the_event_count = 0;
                wd_info.the_event_flag = Persistent_watchdog_info::No_event;
                wd_info.the_sentinel_2 = Persistent_watchdog_info::Sentinel_2;
            }

            optional_watchdog_user = user;
            optional_watchdog_func = func;
            __DSB();
            __ISB();
            // Configure the Reinforced Safety Watchdog Timer (RSWDT) to raise a Watchdog INTERRUPT.
            // SamV71 Errata: "It is not possible to use the RSWDT as a processor Reset source".
            // SamV71 Errata: "The RSWDT counter does not stop when the processor is halted and the WDDBGHLT bit is set".
            WDT1_REGS->WDT_MR =
                    WDT_MR_WDFIEN(1) |
                    WDT_MR_WDD(Reinforced_fixed) | WDT_MR_WDV(Reinforced_counter);
            __DSB();
            __ISB(); // Enable the Reinforced Safety Watchdog Timer interrupt in the NVIC
            enum : uint32_t { RSWDT_interrupt = WDT1_IRQn };
            NVIC->ISER[RSWDT_interrupt >> 5] = 1 << (RSWDT_interrupt & 0x1F);
        }

        void check_watchdog() {
            __DSB();
            __ISB();
            // Reset the primary watchdog.
            WDT_REGS->WDT_CR = WDT_CR_WDRSTT(1) | WDT_CR_KEY(Primary_watchdog_password);
            __DSB();
            __ISB();
            // Reset the reinforced watchdoc.
            WDT1_REGS->WDT_CR = WDT_CR_WDRSTT(1) | WDT_CR_KEY(Reinforced_watchdog_password);
            __DSB();
            __ISB();
        }
    } // SamV71_watchdog
} // SamV71
