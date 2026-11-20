//
// Created by aeols on 12.08.2026.
//

module;

#include <cstdint>
#include <cstdio>
//
// #include "sam.h"
// #include "cachel1_armv7.h"

module Board.AstraLEO_EM_board;
import Type.Abstract_board;
import Board.AstraLEO_EM_pin_table;
import Platform.SamV71_clock;
import Platform.SamV71_I2C;
import Platform.SamV71_SPI;
import Platform.SamV71_USART;
import Platform.SamV71_platform;

namespace Board {
    using namespace SamV71;

    void AstraLEO_EM_board::initialize() {
        the_clock.initialize();
        SamV71_platform::initialize();
        the_pin_table.initialize();
        the_SPI.initialize();
        the_I2C.initialize();
        the_UART.initialize();
        the_ADC.initialize();
        the_DAC.initialize();
    }

    void AstraLEO_EM_board::print_diagnostics() {
        the_SPI.print_diagnostics();
        the_I2C.print_diagnostics();
        the_pin_table.print_diagnostics();
    }

} // Board
