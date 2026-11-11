//
// Created by aeols on 12.08.2026.
//

module;
#include <cstdio>

module Board.Win11_board;

namespace Board {
    void Win11_board::initialize() {
        the_pin_table.initialize();
        the_UART.initialize();
        the_SPI.initialize();
        the_ADC.initialize();
        the_DAC.initialize();
    }

    void Win11_board::print_diagnostics() {
        printf("Board: Win11 (all peripherals are mockups)\r\n");
        the_SPI.print_diagnostics();
        the_pin_table.print_diagnostics();
    }
} // Board
