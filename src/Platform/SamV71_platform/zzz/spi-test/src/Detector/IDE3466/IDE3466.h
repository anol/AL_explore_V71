//
// Created by anolsen on 19.09.2019.
//

#ifndef SPI_TEST_IDE3466_H
#define SPI_TEST_IDE3466_H

#include <spi/SPI_proxy.h>
#include <gpio/GPIO_pin.h>
#include "IDE3466_digital_readout.h"
#include "IDE3466_analog_readout.h"
#include "IDE3466_calibration.h"
#include "IDE3466_register.h"
#include "IDE3466_register_bank.h"


class IDE3466 {
    enum {
        transmission_size = 2
    };
    enum IDE3466_state {
        state_idle, state_remote_reset, state_error
    };
public:
    IDE3466();

    void initialize();

    void on_transaction_finished();

    void on_transaction_failed();

    void read_all_registers();

    void write_all_registers();

    bool is_read_idle() const { return spi_read_registers.is_idle(); }

    bool is_write_idle() const { return spi_write_registers.is_idle(); }

    IDE3466_register_bank &get_register_bank(bool shadow_bank);

    SPI_proxy &get_proxy() { return m_proxy; };

private:
    void reset_remote_SPI();

private:
    IDE3466_state m_state;
    GPIO_pin SRESET_pin;
    SPI_proxy m_proxy;
    IDE3466_register_bank spi_write_registers;
    IDE3466_register_bank spi_read_registers;
    IDE3466_digital_readout digital_readout;
    IDE3466_analog_readout analog_readout;
    IDE3466_calibration calibration;
    uint32_t tx_data[transmission_size] = {0};
    uint32_t rx_data[transmission_size] = {0};
    SPI_proxy::SPI_transaction m_transaction{};
};


#endif //SPI_TEST_IDE3466_H
