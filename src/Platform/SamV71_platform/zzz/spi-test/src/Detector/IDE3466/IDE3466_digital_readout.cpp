//
// Created by anolsen on 20.09.2019.
//

#include <cstdint>
#include "IDE3466_digital_readout.h"

IDE3466_digital_readout::IDE3466_digital_readout(IDE3466_register_bank &bank_write, IDE3466_register_bank &bank_read) :
        m_bank_write(bank_write), m_bank_read(bank_read) {

}

void IDE3466_digital_readout::initialize() {

}
