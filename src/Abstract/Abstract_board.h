//
// Created by aeols on 12.08.2026.
//

#pragma once
#include <cstdint>

#include "Abstract_IO_pin.h"
#include "Abstract_UART.h"

namespace Dictionary
{
    enum Pin_id : std::uint8_t;
}

namespace Abstract
{
    class Abstract_board
    {
    public:
        Abstract_board() = default;

        virtual ~Abstract_board() = default;

        virtual void initialize() = 0;

        virtual Abstract_UART& get_UART() = 0;

        virtual Abstract_IO_pin& get_pin(Dictionary::Pin_id id) = 0;

        virtual void print_diagnostics() = 0;

        virtual void NOP() =0;
    };
} // Abstract
