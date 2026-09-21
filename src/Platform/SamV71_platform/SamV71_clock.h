#pragma once

#include <cstdint>

import Type.Abstract_clock;

namespace SamV71
{
    class SamV71_clock : public Abstract::Abstract_clock
    {
        volatile uint32_t milliseconds_allmost_since_start{};

    public:
        SamV71_clock() = default;

        void initialize() override;

        uint32_t get_milliseconds() override { return milliseconds_allmost_since_start; }

        static uint32_t get_frequency();

        static void enable_peripheral_clock(uint32_t peripheral_id);

    private:
        static void initialize_main_clock();

        static void initialize_PLLA();

        static void initialize_master_clock();

        static void disable_watchdog();
    };
}
