#pragma once

import Type.Abstract_IO_pin;

#include "SamV71_IO_pin.h"
#include "V71_EK_pin_table.h"

namespace SamV71
{
    class V71_EK_pin_manager
    {
        V71_EK_pin_table<SamV71_IO_pin> the_pin_table;

    public:
        using Pin = Abstract::Abstract_IO_pin;

        Status_code initialize();

        Status_code set_phase(Pin::Pin_phase);

        Pin& get_pin(Domain::Pin_id);

        Pin* get_optional_pin(Domain::Pin_id);

        void print_diagnostics();

    private:
        static void initialize_matrix();
    };
}
