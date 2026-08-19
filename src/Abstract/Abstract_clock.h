#pragma once

#include <cstdint>

#include <Utility/Utility_types.h>

namespace Abstract_clock {
    extern void initialize_clocks(Frequency oscillator);

    extern void initialize_timers();

    extern uint32_t get_milliseconds();

    extern uint32_t get_master_clock();

    extern uint32_t get_millisecond_timestamp();

    extern uint64_t get_microsecond_timestamp();

    extern uint32_t get_hundredthsecond_timestamp();
}
