//
// Created by anolsen on 20.09.2019.
//

#ifndef SPI_TEST_IDE3466_REGISTER_H
#define SPI_TEST_IDE3466_REGISTER_H


#include <spi/SPI_proxy.h>
#include <Consol.h>


typedef void (*for_each_register)(void *p_user, int reg_number, uint32_t data, const char *name);

template<uint32_t base_address, bool read_only, uint32_t number_of_registers>
class IDE3466_register {
    enum {
        Register_data_mask = 0x3FFFFFu,
        Register_rw_shift = 22u,
        Register_rw_mask = (0x01u << Register_rw_shift),
        Register_address_shift = 23u,
        Register_address_mask = (0x01FFu << Register_address_shift)
    };
public:
    void initialize(bool set_write_command, uint32_t default_value = 0) {
        for (uint32_t reg_number = 0; reg_number < m_number_of_registers; reg_number++) {
            tx_data[reg_number] =
                    ((m_base_address + reg_number) << Register_address_shift) |
                    (set_write_command ? (0x1u << Register_rw_shift) : 0u) |
                    (Register_data_mask & default_value);
            rx_data[reg_number] = 0xABBABABEu;
        }
    }

    void set_tx_data(uint32_t reg_number, uint32_t data) {
        if (reg_number < m_number_of_registers) {
            tx_data[reg_number] &= ~Register_data_mask;
            tx_data[reg_number] |= Register_data_mask & data;
        }
    }

    uint32_t get_tx_data(uint32_t reg_number) {
        if (reg_number < m_number_of_registers) {
            return Register_data_mask & (tx_data[reg_number]);
        } else {
            return 0;
        }
    }

    uint32_t get_rx_data(uint32_t reg_number) {
        if (reg_number < m_number_of_registers) {
            return Register_data_mask & (rx_data[reg_number]);
        } else {
            return 0;
        }
    }

    uint32_t get_tx(uint32_t reg_number) {
        if (reg_number < m_number_of_registers) {
            return (tx_data[reg_number]);
        } else {
            return 0;
        }
    }

    uint32_t get_rx(uint32_t reg_number) {
        if (reg_number < m_number_of_registers) {
            return (rx_data[reg_number]);
        } else {
            return 0;
        }
    }

    void for_each_tx(void *p_user, for_each_register callback) const {
        for (uint32_t reg_number = 0; reg_number < m_number_of_registers; reg_number++) {
            callback(p_user, reg_number, tx_data[reg_number], m_name);
        }
    }

    void for_each_rx(void *p_user, for_each_register callback) const {
        for (uint32_t reg_number = 0; reg_number < m_number_of_registers; reg_number++) {
            callback(p_user, reg_number, rx_data[reg_number], m_name);
        }
    }

    bool read(SPI_proxy &rProxy, SPI_proxy::SPI_transaction transaction) {
        Consol::global_printf("Reading the %s.\r\n", m_name);
        transaction.tx_buffer = tx_data;
        transaction.rx_buffer = rx_data;
        transaction.size = m_number_of_registers;
        return rProxy.start_transaction(transaction);
    }

    bool write(SPI_proxy &rProxy, SPI_proxy::SPI_transaction transaction) {
        if (m_read_only) {
            Consol::global_printf("Sorry, the %s is read-only.\r\n", m_name);
            transaction.failed(transaction.p_user);
            return false;
        } else {
            Consol::global_printf("Writing the %s.\r\n", m_name);
            transaction.tx_buffer = tx_data;
            transaction.rx_buffer = rx_data;
            transaction.size = m_number_of_registers;
            return rProxy.start_transaction(transaction);
        }
    }

    IDE3466_register(const char *tag, const char *name, const char *brief) :
            m_tag(tag), m_name(name), m_brief(brief) {}

private:
    const char *m_tag{};
    const char *m_name{};
    const char *m_brief{};
    const bool m_read_only{read_only};
    const uint32_t m_base_address{base_address};
    const uint32_t m_number_of_registers{number_of_registers};
    uint32_t tx_data[number_of_registers]{};
    uint32_t rx_data[number_of_registers]{};
};


#endif //SPI_TEST_IDE3466_REGISTER_H
