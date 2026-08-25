//
// Created by aeols on 12.08.2026.
//

#pragma once
#include "Abstract_board.h"
#include "SamV71_clock.h"
#include "SamV71_pin_manager.h"
#include "SamV71_UART.h"

namespace Board {
    using namespace SamV71;

    class V71_EK_board : public Abstract::Abstract_board {
        SamV71_clock the_clock;
        SamV71_pin_manager the_pin_manager;
        SamV71_UART the_UART;


    public:
        void initialize() override;

        Abstract_UART &get_UART() { return the_UART; }

        Abstract::Abstract_IO_pin &get_pin(const Pin_id id) {
            return the_pin_manager.get_pin(id);
        }

        Abstract::Abstract_IO_pin *get_optional_pin(Pin_id) {
            return nullptr;
        };
    };
} // Board
