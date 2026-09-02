//
// Created by anolsen on 19.09.2019.
//

#include <cstdint>
#include <samv71q21b_pio.h>
#include "IDE3466_calibration.h"

IDE3466_calibration::IDE3466_calibration(IDE3466_register_bank &bank_write, IDE3466_register_bank &bank_read) :
        m_bank_write(bank_write),
        m_bank_read(bank_read),
        DCAL_pin(PIO_PD19_IDX, true),
        MSTEST_pin(PIO_PD28_IDX, false) {}

void IDE3466_calibration::initialize() {
    DCAL_pin.clear();
    DCAL_pin.enable();
    MSTEST_pin.enable();
}
