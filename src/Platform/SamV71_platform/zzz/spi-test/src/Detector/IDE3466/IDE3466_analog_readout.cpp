//
// Created by anolsen on 19.09.2019.
//

#include <cstdint>
#include <samv71q21b_pio.h>
#include "IDE3466_analog_readout.h"

IDE3466_analog_readout::IDE3466_analog_readout(IDE3466_register_bank &bank_write, IDE3466_register_bank &bank_read) :
        m_bank_write(bank_write),
        m_bank_read(bank_read),
        DRESET_pin(PIO_PD27_IDX, true),
        DTVO_BUF_pin(PIO_PA18_IDX, false),
        RO_CLK_pin(PIO_PA6_IDX, true),
        SHIFT_IN_pin(PIO_PC19_IDX, true),
        SHIFT_OUT_pin(PIO_PC12_IDX, false),
        TLGVO_BUF_pin(PIO_PB0_IDX, false) {}

void IDE3466_analog_readout::initialize() {
    DRESET_pin.clear();
    RO_CLK_pin.clear();
    SHIFT_IN_pin.clear();
    DRESET_pin.enable();
    DTVO_BUF_pin.enable();
    RO_CLK_pin.enable();
    SHIFT_IN_pin.enable();
    SHIFT_OUT_pin.enable();
    TLGVO_BUF_pin.enable();
}
