//
// Created by anolsen on 19.09.2019.
//

#ifndef SPI_TEST_IDE3466_ANALOG_READOUT_H
#define SPI_TEST_IDE3466_ANALOG_READOUT_H

#include <gpio/GPIO_pin.h>
#include "IDE3466_register_bank.h"

class IDE3466_analog_readout {
public:
    IDE3466_analog_readout(IDE3466_register_bank &bank_write, IDE3466_register_bank &bank_read);

    void initialize();

private:
    IDE3466_register_bank &m_bank_write;
    IDE3466_register_bank &m_bank_read;

private:
    GPIO_pin DRESET_pin;
    GPIO_pin DTVO_BUF_pin;
    GPIO_pin RO_CLK_pin;
    GPIO_pin SHIFT_IN_pin;
    GPIO_pin SHIFT_OUT_pin;
    GPIO_pin TLGVO_BUF_pin;
};

#endif //SPI_TEST_IDE3466_ANALOG_READOUT_H
