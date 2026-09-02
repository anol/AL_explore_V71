//
// Created by aeols on 2026-09-02.
//

#pragma once
#include "Abstract_IO_pin.h"
#include "IO_pins.h"

namespace Win11
{
    class Win11_pin_manager
    {
    public :
        using Pin = Abstract::Abstract_IO_pin;

        bool initialize();

        bool set_phase(Pin::Pin_phase);

        Pin& get_pin(Dictionary::Pin_id);

        Pin* get_optional_pin(Dictionary::Pin_id);

        void print_diagnostics();
    };
} // Win11
