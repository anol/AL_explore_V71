//
// Created by aeols on 2026-09-02.
//

#pragma once
import Type.Abstract_IO_pin;
import Domain.IO_pins;

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
