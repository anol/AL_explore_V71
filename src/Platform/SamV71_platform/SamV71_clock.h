#pragma once

#include "Abstract_clock.h"

namespace SamV71_clock {
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
