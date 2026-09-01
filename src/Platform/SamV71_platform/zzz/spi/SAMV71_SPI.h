/*
 * Copyright (C) 2021 Integrated Detector Electronics AS
 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains
 * the property of Integrated Detector Electronics AS and its suppliers,
 * if any. The intellectual and technical concepts contained
 * herein are proprietary to Integrated Detector Electronics AS
 * and its suppliers and may be covered by Norwegian, EU. or U.S. patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Integrated Detector Electronics AS.
 */

/**
 * \date   IDEAS/23.08.2019/aeols
 * \brief
 */

#ifndef NORM_FW_SPI_PROXY_H
#define NORM_FW_SPI_PROXY_H

#include <component/spi.h>
#include <Ringbuffer.h>
#include <SPI_interface.h>
#include <component/gpio/SAMV71_GPIO.h>

class SAMV71_SPI : public SPI_interface {
    typedef Ringbuffer<SPI_transaction, 128> Request_buffer;

public:
    class SPI_definition {
    public:
        int SPI_number;
        uint32_t peripheral_id;
        Spi *p_SPI;
        SAMV71_GPIO::gpio_port miso_port;
        uint32_t miso_pin;
        SAMV71_GPIO::gpio_function miso_mode;
        SAMV71_GPIO::gpio_port mosi_port;
        uint32_t mosi_pin;
        SAMV71_GPIO::gpio_function mosi_mode;
        SAMV71_GPIO::gpio_port clock_port;
        uint32_t clock_pin;
        SAMV71_GPIO::gpio_function clock_mode;
        SAMV71_GPIO::gpio_port nss_port;
        uint32_t nss_pin;
    };

    void on_interrupt();

private:
    bool the_busy_flag{};
    SPI_definition the_definition;
    SPI_transaction the_transaction{};
    SAMV71_GPIO MISO_pin;
    SAMV71_GPIO MOSI_pin;
    SAMV71_GPIO Clock_pin;
    SAMV71_GPIO CS0_pin;
    uint32_t the_before_timestamp{};
    uint32_t the_after_timestamp{};
    Request_buffer the_request_queue;

public:
    explicit SAMV71_SPI(const SPI_definition &definition);

    void initialize(uint32_t bitrate);

    bool start_transaction(const SPI_transaction &transaction) override;

    bool is_idle() override { return the_request_queue.is_empty(); }

    bool on_tx_ready(uint8_t *p_data);

    bool on_rx_ready(uint8_t data);

    void reset() override ;

    void report(Service_report &reporter) override;

private:
    bool is_busy() const { return the_busy_flag; }

    bool is_ready() const { return !the_busy_flag; }

    bool is_transaction_complete();

    void disable_SPI() const;

    void enable_SPI() const;

    void execute_pending_transaction();

    void set_busy() { the_busy_flag = true; };

    void setup_SPI_registers(uint32_t bitrate_divider) const;

    void set_ready() { the_busy_flag = false; };

    void end_transaction(bool success);
};

#endif //NORM_FW_SPI_PROXY_H