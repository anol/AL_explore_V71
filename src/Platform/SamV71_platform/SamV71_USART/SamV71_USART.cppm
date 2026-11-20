//
// Created by aeols on 2026-08-24.
//

module;

#include <cstdint>

export module Platform.SamV71_USART;
import :Config;
import :Driver;
import :ISR;

import Type.Abstract_UART;
import Type.Misc_type;
import Platform.SamV71_clock;

export namespace SamV71
{
    class SamV71_USART : public Abstract::Abstract_UART
    {
        const uint8_t the_id;
        bool the_echo_flag{};
        SamV71_USART_driver the_driver;
        SamV71_USART_ISR the_ISR;

    public:
        explicit SamV71_USART(uint8_t id)
            : the_id(id),
              the_driver(SamV71_USART_config::get_config(id)),
              the_ISR(SamV71_USART_config::get_config(id), the_driver)
        {
        }

        void initialize() override;

        bool is_ready() override { return true; }

        Status_code put(const uint8_t data) override { return Status_code(the_driver.send(data)); }

        Status_code get(uint8_t* data) override { return Status_code(the_driver.receive(data)); }

        bool for_each_input(Optional_user user, Optional_func func) override;

        int print(const char* data, const int len) override { return the_driver.print(data, len); }

        bool toggle_echo() override { return the_echo_flag = !the_echo_flag; }

        void print_diagnostics() const { the_driver.print_diagnostics(); }

        [[nodiscard]] uint8_t get_id() const { return the_id; }

    private:
        [[nodiscard]] bool is_echo() const { return the_echo_flag; }
    };

    void SamV71_USART::initialize()
    {
        if (const auto* config = SamV71_USART_config::get_config(the_id))
        {
            SamV71_clock::enable_peripheral_clock(config->the_peripheral_id);
            the_driver.setup_USART_registers();
            the_driver.enable_USART();
        }
    }

    bool SamV71_USART::for_each_input(Optional_user user, Optional_func func)
    {
        uint8_t data;
        while (the_driver.receive(&data))
        {
            if (is_echo())
            {
                (void)the_driver.send(data);
            }
            if (func)
            {
                func(user, data);
            }
        }
        if (is_echo())
        {
            the_driver.enable_transmitter_interrupt();
        }
        return true;
    }
} // SamV71
