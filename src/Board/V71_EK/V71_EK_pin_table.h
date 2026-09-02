#pragma once
#include "IO_pins.h"

namespace SamV71 {
    template<typename IO_pin>
    class V71_EK_pin_table {
        using Tag = Dictionary::Pin_id;
        using Pin = Abstract::Abstract_IO_pin;

    public:
        IO_pin the_pins[Tag::Number_of_pins]{
            // Pin name, Function, Phase, Port, Pin, Mode, Type, Default strength=0, Default state=low
            {Tag::Pin_not_used},

            {Tag::Pin_SDA, "SDA", Pin::Start_up, Pin::Port_A, 3, Pin::Mode_A, Pin::Input_normal},                      // 91
            {Tag::Pin_SCL, "SCL", Pin::Start_up, Pin::Port_A, 4, Pin::Mode_A, Pin::Input_normal},                      // 77
            {Tag::Pin_ASIC_RESET_I, "ASIC_RESET_I", Pin::Start_up, Pin::Port_C, 0, Pin::Mode_GPIO, Pin::Out_normal},   // 11
            {Tag::Pin_ASIC_HOLD_I, "ASIC_HOLD_I", Pin::Start_up, Pin::Port_C, 1, Pin::Mode_GPIO, Pin::Out_normal},     // 38
            {Tag::Pin_ASIC_CLKENEXT, "ASIC_CLKENEXT", Pin::Start_up, Pin::Port_C, 2, Pin::Mode_GPIO, Pin::Out_normal}, // 39
            {Tag::Pin_ASIC_TOT_EN, "ASIC_TOT_EN", Pin::Start_up, Pin::Port_C, 3, Pin::Mode_GPIO, Pin::Out_normal},     // 40
            {Tag::Pin_SPI_CS_ASIC5, "SPI_CS_ASIC5", Pin::Start_up, Pin::Port_C, 12, Pin::Mode_GPIO, Pin::Out_normal},  // 17
            {Tag::Pin_SPI_CS_ASIC4, "SPI_CS_ASIC4", Pin::Start_up, Pin::Port_C, 13, Pin::Mode_GPIO, Pin::Out_normal},  // 19
            {Tag::Pin_SPI_CS_ASIC3, "SPI_CS_ASIC3", Pin::Start_up, Pin::Port_C, 14, Pin::Mode_GPIO, Pin::Out_normal},  // 97
            {Tag::Pin_SPI_CS_ASIC2, "SPI_CS_ASIC2", Pin::Start_up, Pin::Port_C, 15, Pin::Mode_GPIO, Pin::Out_normal},  // 18
            {Tag::Pin_SPI_CS_ASIC1, "SPI_CS_ASIC1", Pin::Start_up, Pin::Port_C, 16, Pin::Mode_GPIO, Pin::Out_normal},  // 100
            {Tag::Pin_SPI_CS_DAC1, "SPI_CS_DAC1", Pin::Start_up, Pin::Port_C, 19, Pin::Mode_GPIO, Pin::Out_normal},    // 117
            {Tag::Pin_SPI_CS_DAC0, "SPI_CS_DAC0", Pin::Start_up, Pin::Port_C, 20, Pin::Mode_GPIO, Pin::Out_normal},    // 120
            {Tag::Pin_FPGA_CS_N, "FPGA_CS_N", Pin::Start_up, Pin::Port_C, 22, Pin::Mode_GPIO, Pin::Out_normal},        // 124
            {Tag::Pin_FPGA_IRQ, "FPGA_IRQ", Pin::Start_up, Pin::Port_C, 23, Pin::Mode_GPIO, Pin::Input_normal},        // 127
            {Tag::Pin_FPGA_CMD, "FPGA_CMD", Pin::Start_up, Pin::Port_C, 24, Pin::Mode_GPIO, Pin::Out_normal},          // 130
            {Tag::Pin_FPGA_BUSY, "FPGA_BUSY", Pin::Start_up, Pin::Port_C, 25, Pin::Mode_GPIO, Pin::Input_normal},      // 133
            {Tag::Pin_RMII_CLKOUT, "RMII_CLKOUT", Pin::Never_up, Pin::Port_D, 0, Pin::Mode_A, Pin::Input_normal},      // 1
            {Tag::Pin_RMII_TX_EN, "RMII_TX_EN", Pin::Never_up, Pin::Port_D, 1, Pin::Mode_A, Pin::Input_normal},        // 132
            {Tag::Pin_RMII_TXD0, "RMII_TXD0", Pin::Never_up, Pin::Port_D, 2, Pin::Mode_A, Pin::Input_normal},          // 133
            {Tag::Pin_RMII_TXD1, "RMII_TXD1", Pin::Never_up, Pin::Port_D, 3, Pin::Mode_A, Pin::Input_normal},          // 128
            {Tag::Pin_RMII_CRSDV, "RMII_CRSDV", Pin::Never_up, Pin::Port_D, 4, Pin::Mode_A, Pin::Input_normal},        // 126
            {Tag::Pin_RMII_RXD0, "RMII_RXD0", Pin::Never_up, Pin::Port_D, 5, Pin::Mode_A, Pin::Input_normal},          // 125
            {Tag::Pin_RMII_RXD1, "RMII_RXD1", Pin::Never_up, Pin::Port_D, 6, Pin::Mode_A, Pin::Input_normal},          // 121
            {Tag::Pin_RMII_MDC, "RMII_MDC", Pin::Never_up, Pin::Port_D, 8, Pin::Mode_A, Pin::Input_normal},            // 113
            {Tag::Pin_RMII_MDIO, "RMII_MDIO", Pin::Never_up, Pin::Port_D, 9, Pin::Mode_A, Pin::Input_normal},          // 110
            {Tag::Pin_SPI_MISO, "SPI_MISO", Pin::Start_up, Pin::Port_D, 20, Pin::Mode_B, Pin::Input_normal},           // 65
            {Tag::Pin_SPI_MOSI, "SPI_MOSI", Pin::Start_up, Pin::Port_D, 21, Pin::Mode_B, Pin::Input_normal},           // 63
            {Tag::Pin_SPI_SCK, "SPI_SCK", Pin::Start_up, Pin::Port_D, 22, Pin::Mode_B, Pin::Input_normal},             // 60
            {Tag::Pin_EOUT_MON, "EOUT_MON", Pin::Start_up, Pin::Port_D, 30, Pin::Mode_GPIO, Pin::Input_normal},        // 34
            {Tag::Pin_UART_RXD1, "UART_RXD1", Pin::Start_up, Pin::Port_A, 21, Pin::Mode_A, Pin::Input_normal},         // 32
            {Tag::Pin_UART_TXD1, "UART_TXD1", Pin::Start_up, Pin::Port_B, 4, Pin::Mode_D, Pin::Out_normal},          // 105
            {Tag::Pin_LED0, "LED0", Pin::Start_up, Pin::Port_A, 23, Pin::Mode_GPIO, Pin::Out_normal},                  // 46
            {Tag::Pin_LED1, "LED1", Pin::Start_up, Pin::Port_C, 9, Pin::Mode_GPIO, Pin::Out_normal},                   // 86
            {Tag::Pin_ALT_WKUP6, "ALT_WKUP6", Pin::Start_up, Pin::Port_A, 9, Pin::Mode_GPIO, Pin::Input_normal},       // 75
            {Tag::Pin_TEMP_ALERT, "TEMP_ALERT", Pin::Start_up, Pin::Port_E, 0, Pin::Mode_GPIO, Pin::Input_normal},     // 144

        };
    };
}
