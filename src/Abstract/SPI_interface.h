//
// Created by anolsen on 23.08.2019.
//
#ifndef NORM_FW_SPI_interface_H
#define NORM_FW_SPI_interface_H

#include <cstdint>
#include "Transaction/Service_report.h"

/// Purpose: Hardware abstraction of the MCU SPI peripheral.
class SPI_interface {
public:
    typedef void (*SPI_callback_t)(void *p_user, bool success, uint32_t before_timestamp, uint32_t after_timestamp);

    struct SPI_transaction {
        void *optional_user{};
        SPI_callback_t optional_callback{};
        volatile uint32_t *tx_buffer{};
        volatile uint32_t *rx_buffer{};
        uint8_t the_word_size{};
        uint8_t the_word_count{};
        uint8_t chip_select{};
        bool the_write_flag{};

        SPI_transaction() = default;

        SPI_transaction(void *user, SPI_callback_t callback) : optional_user(user), optional_callback(callback) {}
    };

    enum {
        Register_data_mask = 0x3FFFFFu,
        Register_rw_shift = 22u,
        Register_rw_mask = (0x01u << Register_rw_shift),
        Register_address_shift = 23u,
        Register_address_mask = (0x01FFu << Register_address_shift)
    };

public:
    virtual void initialize() = 0;

    virtual bool start_transaction(const SPI_transaction &) = 0;

    virtual bool initiate_pending_transaction() =0;

    virtual bool is_idle() { return true; }

    virtual void reset() {}

    virtual void report_state(Service_report *) const {}
};

#endif //NORM_FW_SPI_interface_H