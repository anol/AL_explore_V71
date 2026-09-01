//
// Created by anolsen on 23.08.2019.
//
#ifndef NORM_FW_SAMV71_UART_H
#define NORM_FW_SAMV71_UART_H

#include <component/usart.h>
#include <component/gpio/SAMV71_GPIO.h>
#include <Ringbuffer.h>
#include <UART_interface.h>

class SAMV71_UART : public UART_interface {
public:
    class Usart_definition {
    public:
        Usart *p_USART;
        int USART_number;
        uint32_t irq_number;
        SAMV71_GPIO::gpio_port rx_port;
        uint32_t rx_pin;
        SAMV71_GPIO::gpio_function rx_mode;
        SAMV71_GPIO::gpio_port tx_port;
        uint32_t tx_pin;
        SAMV71_GPIO::gpio_function tx_mode;
    };

    void rx(uint8_t data);

    bool tx(uint8_t *p_data);

    void error(uint32_t diag);

    bool has_input() override;

    bool for_each_input(Optional_user user, Optional_func func) override {
        return m_rx_buffer.for_each(user, func);
    }

    explicit SAMV71_UART(const Usart_definition &definition);

    void initialize(uint32_t bitrate) override;

    bool is_ready() override;

    int print(const char *ptr, int len) override;

    int put(uint8_t c) override;

    bool get(uint8_t *p_data) override;

private:
    uint32_t setup_UART(uint32_t bitrate);

    uint32_t set_bitrate(uint32_t bitrate) const;

    void reset() const;

    void setup_interrupts();

private:
    Usart_definition m_definition;
    int m_overflow_count{0};
    SAMV71_GPIO rx_pin;
    SAMV71_GPIO tx_pin;
    UART_transmit_buffer m_tx_buffer{};
    UART_receive_buffer m_rx_buffer{};
};

#endif //NORM_FW_SAMV71_UART_H