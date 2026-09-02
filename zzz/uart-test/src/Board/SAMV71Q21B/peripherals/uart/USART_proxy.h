//
// Created by anolsen on 23.08.2019.
//
#ifndef PM_V71_USART_PROXY_H
#define PM_V71_USART_PROXY_H

#include <component/usart.h>
#include <Ringbuffer.h>


class USART_proxy {
public:
    class Usart_definition {
    public:
        Usart *p_USART;
        int USART_number;
        uint32_t irq_number;
        uint32_t rx_pin;
        uint32_t rx_mode;
        uint32_t tx_pin;
        uint32_t tx_mode;
    };

    void rx(uint8_t data);

    bool tx(uint8_t *p_data);

    void error(uint32_t diag);

    bool has_input();

    int for_each_input(void *p_user, receiver_t p_receiver);

public:
    explicit USART_proxy(const Usart_definition &definition);

    void initialize();

public:
    int print(const char *ptr, int len);

    int put(char c);

private:
    uint32_t init_rs232(uint32_t ul_mck);

    uint32_t set_bitrate(uint32_t bitrate, uint32_t ul_mck);

    void reset();

    void init_interrupts();

private:
    Usart_definition m_definition;
    Ringbuffer m_tx_buffer;
    Ringbuffer m_rx_buffer;

};

#endif //PM_V71_USART_PROXY_H