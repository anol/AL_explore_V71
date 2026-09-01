//
// Created by anolsen on 19.09.2019.
//

#include <samv71q21b.h>
#include <samv71q21b_pio.h>

#include "IDE3466.h"

enum {
    SPI_bitrate = 1000000
};

const SPI_proxy::SPI_definition definition = {
        SPI0, ID_SPI0, SPI0_IRQn,
        PIO_PD20_IDX, PERIPHERAL_MODE_MUX_B,
        PIO_PD21_IDX, PERIPHERAL_MODE_MUX_B,
        PIO_PD22_IDX, PERIPHERAL_MODE_MUX_B,
        PIO_PD25_IDX
};

IDE3466::IDE3466() :
        m_state(state_idle),
        SRESET_pin(PIO_PA5_IDX, true, GPIO_MODE),
        m_proxy(definition),
        spi_write_registers(m_proxy),
        spi_read_registers(m_proxy),
        digital_readout(spi_write_registers, spi_read_registers),
        analog_readout(spi_write_registers, spi_read_registers),
        calibration(spi_write_registers, spi_read_registers) {}

void IDE3466::initialize() {
    SRESET_pin.clear();
    SRESET_pin.enable();
    m_proxy.initialize(SPI_bitrate);
    spi_write_registers.initialize(true);
    spi_read_registers.initialize(false);
    digital_readout.initialize();
    analog_readout.initialize();
    calibration.initialize();
    reset_remote_SPI();
}

static void on_transaction_finished_cb(void *p_user) {
    ((IDE3466 *) p_user)->on_transaction_finished();
}

static void on_transaction_failed_cb(void *p_user) {
    ((IDE3466 *) p_user)->on_transaction_failed();
}

void IDE3466::reset_remote_SPI() {
    m_transaction.finish = on_transaction_finished_cb;
    m_transaction.failed = on_transaction_failed_cb;
    m_transaction.tx_buffer = tx_data;
    m_transaction.rx_buffer = rx_data;
    m_transaction.size = transmission_size;
    m_transaction.p_user = this;
    SRESET_pin.set();
    m_state = state_remote_reset;
    if (!m_proxy.start_transaction(m_transaction)) {
        m_state = state_error;
    }
}

void IDE3466::read_all_registers() {
    spi_read_registers.read_all();
    while (!is_read_idle());
}

void IDE3466::write_all_registers() {
    spi_write_registers.write_all();
    while (!is_read_idle());
}

void IDE3466::on_transaction_finished() {
    switch (m_state) {
        default:
            m_state = state_idle;
            break;
        case state_idle:
            break;
        case state_remote_reset:
            SRESET_pin.clear();
            m_state = state_idle;
            break;
    }
}

void IDE3466::on_transaction_failed() {
    m_state = state_error;
}

IDE3466_register_bank &IDE3466::get_register_bank(bool shadow_bank) {
    if (shadow_bank) {
        return spi_read_registers;
    } else {
        return spi_write_registers;
    }
}

