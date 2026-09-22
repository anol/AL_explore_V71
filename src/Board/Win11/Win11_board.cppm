//
// Created by aeols on 12.08.2026.
//

module;
#include "Win11_pin_manager.h"
#include "Win11_UART.h"

export module Board.Win11_board;
import Type.Abstract_board;


export namespace Board {
    class Win11_board : public Abstract::Abstract_board {
        Win11::Win11_pin_manager the_pin_manager{};
        Win11::Win11_UART the_UART{};
        Win11::Win11_SPI the_SPI{};

    public:
        void initialize() override;

        Abstract::Abstract_SPI &get_SPI() override { return the_SPI; };

        Abstract::Abstract_UART &get_UART() override { return the_UART; }

        Abstract::Abstract_IO_pin &get_pin(Abstract::Abstract_pin_id id) override {
            return the_pin_manager.get_pin(id);
        }

        void print_diagnostics() override;
    };
} // Board
