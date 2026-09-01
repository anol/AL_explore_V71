//
// Created by anolsen on 23.08.2019.
//
#ifndef PM_V71_SPI_PROXY_H
#define PM_V71_SPI_PROXY_H

#include <component/spi.h>
#include <gpio/GPIO_pin.h>
#include <Ringbuffer.h>

typedef void (*transaction_finish_t)(void *p_user);

typedef void (*transaction_failed_t)(void *p_user);

class SPI_proxy {
public:
    class SPI_definition {
    public:
        Spi *p_SPI;
        int SPI_number;
        uint32_t irq_number;
        uint32_t miso_pin;
        gpio_function miso_mode;
        uint32_t mosi_pin;
        gpio_function mosi_mode;
        uint32_t clock_pin;
        gpio_function clock_mode;
        uint32_t nss_pin;
    };

    class SPI_transaction {
    public:
        transaction_finish_t finish;
        transaction_failed_t failed;
        void *p_user;
        uint32_t *tx_buffer;
        uint32_t *rx_buffer;
        int size;
        int tx_count;
        int rx_count;
    };

public:
    explicit SPI_proxy(const SPI_definition &definition);

    void initialize(uint32_t bitrate);

    bool start_transaction(const SPI_transaction &transaction);

    void tx_empty();

    bool tx_ready(uint8_t *p_data);

    bool rx_ready(uint8_t data);

    void transaction_error(uint32_t diag);

private:
    void set_chip_select_register(uint32_t bitrate);

    bool is_transaction_complete();

private:
    SPI_definition m_definition;
    SPI_transaction m_transaction;
    GPIO_pin MISO_pin;
    GPIO_pin MOSI_pin;
    GPIO_pin Clock_pin;
    GPIO_pin Select_pin;
};

#endif //PM_V71_SPI_PROXY_H