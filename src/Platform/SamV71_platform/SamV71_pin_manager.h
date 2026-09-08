#pragma once

#include "Abstract_IO_pin.h"
#include "SamV71_IO_pin.h"
#include "V71_EK_pin_table.h"

namespace SamV71
{
    class SamV71_pin_manager
    {
        V71_EK_pin_table<SamV71_IO_pin> the_pin_table;

    public:
        using Pin = Abstract::Abstract_IO_pin;

        bool initialize();

        bool set_phase(Pin::Pin_phase);

        Pin& get_pin(Dictionary::Pin_id);

        Pin* get_optional_pin(Dictionary::Pin_id);

        void print_diagnostics();

    private:
        static void initialize_matrix();
    };
}
