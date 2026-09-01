//
// Created by anolsen on 20.09.2019.
//

#ifndef SPI_TEST_IDE3466_DIGITAL_READOUT_H
#define SPI_TEST_IDE3466_DIGITAL_READOUT_H


#include "IDE3466_register_bank.h"

class IDE3466_digital_readout {

public:
    IDE3466_digital_readout(IDE3466_register_bank &bank_write, IDE3466_register_bank &bank_read);

    void initialize();

private:
    IDE3466_register_bank &m_bank_write;
    IDE3466_register_bank &m_bank_read;
};


#endif //SPI_TEST_IDE3466_DIGITAL_READOUT_H
