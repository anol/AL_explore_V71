//
// Created by aeols on 2026-09-11.
//

#pragma once

namespace Support
{
    using Pin = Abstract::Abstract_IO_pin;

    class Abstract_pin_table
    {
    public:
        virtual ~Abstract_pin_table() = default;
        virtual Pin& get_pin(Dictionary::Pin_id id) = 0;
        virtual Status_code set_phase(Pin::Pin_phase) = 0;
        virtual Status_code for_each_pin(void (*)(Pin&)) = 0;
        virtual void print_diagnostics() const = 0;
    };
} // Support
