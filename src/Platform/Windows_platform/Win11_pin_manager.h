//
// Created by aeols on 2026-09-02.
//

#pragma once
#include "Abstract_IO_pin.h"
#include "IO_pins.h"
#include "Status_code.h"

namespace Win11
{
    class Win11_pin_manager
    {
    public :
        using Pin = Abstract::Abstract_IO_pin;

        Status_code initialize();

        Status_code set_phase(Pin::Pin_phase);

        Pin& get_pin(Dictionary::Pin_id);

        Pin* get_optional_pin(Dictionary::Pin_id);

        void print_diagnostics();
    };
} // Win11
