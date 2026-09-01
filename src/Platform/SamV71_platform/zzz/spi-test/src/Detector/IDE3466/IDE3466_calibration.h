//
// Created by anolsen on 19.09.2019.
//

#ifndef SPI_TEST_IDE3466_CALIBRATION_H
#define SPI_TEST_IDE3466_CALIBRATION_H

#include <gpio/GPIO_pin.h>
#include "IDE3466_register_bank.h"

class IDE3466_calibration {
public:
    IDE3466_calibration(IDE3466_register_bank &bank_write, IDE3466_register_bank &bank_read);

    void initialize();

private:
    IDE3466_register_bank &m_bank_write;
    IDE3466_register_bank &m_bank_read;

private:
    GPIO_pin DCAL_pin;
    GPIO_pin MSTEST_pin;
};

#endif //SPI_TEST_IDE3466_CALIBRATION_H
