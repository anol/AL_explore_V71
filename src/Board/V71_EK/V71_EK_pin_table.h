#pragma once

import Type.Abstract_IO_pin;
import Domain.IO_pins;

namespace SamV71 {
    template<typename IO_pin>
    class V71_EK_pin_table {
        using Tag = Domain::Pin_id;
        using Pin = Abstract::Abstract_IO_pin;

    public:
        IO_pin the_pins[Tag::Number_of_pins]{
            // Pin name, Function, Phase, Port, Pin, Mode, Type, Default strength=0, Default state=low
            {Tag::Pin_not_used},
            {Tag::Pin_SDA},
            {Tag::Pin_SCL},
            {Tag::Pin_ASIC_RESET_I},
            {Tag::Pin_ASIC_HOLD_I},
            {Tag::Pin_ASIC_CLKENEXT},
            {Tag::Pin_ASIC_TOT_EN},
            {Tag::Pin_SPI_CS_ASIC5, "SPI_CS_ASIC5", Pin::Never_up, Pin::Port_C, 12, Pin::Mode_GPIO, Pin::Out_normal}, // 17
            {Tag::Pin_SPI_CS_ASIC4, "SPI_CS_ASIC4", Pin::Never_up, Pin::Port_C, 13, Pin::Mode_GPIO, Pin::Out_normal}, // 19
            {Tag::Pin_SPI_CS_ASIC3, "SPI_CS_ASIC3", Pin::Never_up, Pin::Port_C, 14, Pin::Mode_GPIO, Pin::Out_normal}, // 97
            {Tag::Pin_SPI_CS_ASIC2, "SPI_CS_ASIC2", Pin::Never_up, Pin::Port_C, 15, Pin::Mode_GPIO, Pin::Out_normal}, // 18
            {Tag::Pin_SPI_CS_ASIC1, "SPI_CS_ASIC1", Pin::Never_up, Pin::Port_C, 16, Pin::Mode_GPIO, Pin::Out_normal}, // 100
            {Tag::Pin_SPI_CS_DAC1, "SPI_CS_DAC1", Pin::Never_up, Pin::Port_C, 19, Pin::Mode_GPIO, Pin::Out_normal},   // 117
            {Tag::Pin_SPI_CS_DAC0, "SPI_CS_DAC0", Pin::Never_up, Pin::Port_C, 20, Pin::Mode_GPIO, Pin::Out_normal},   // 120
            {Tag::Pin_FPGA_CS_N},
            {Tag::Pin_FPGA_IRQ},
            {Tag::Pin_FPGA_CMD},
            {Tag::Pin_FPGA_BUSY},
            {Tag::Pin_RMII_CLKOUT},
            {Tag::Pin_RMII_TX_EN},
            {Tag::Pin_RMII_TXD0},
            {Tag::Pin_RMII_TXD1},
            {Tag::Pin_RMII_CRSDV},
            {Tag::Pin_RMII_RXD0},
            {Tag::Pin_RMII_RXD1},
            {Tag::Pin_RMII_MDC},
            {Tag::Pin_RMII_MDIO},
            {Tag::Pin_SPI_MISO, "SPI_MISO", Pin::Never_up, Pin::Port_D, 20, Pin::Mode_B, Pin::Input_normal}, // 65
            {Tag::Pin_SPI_MOSI, "SPI_MOSI", Pin::Never_up, Pin::Port_D, 21, Pin::Mode_B, Pin::Input_normal}, // 63
            {Tag::Pin_SPI_SCK, "SPI_SCK", Pin::Never_up, Pin::Port_D, 22, Pin::Mode_B, Pin::Input_normal},   // 60
            {Tag::Pin_EOUT_MON},
            {Tag::Pin_UART_RXD1, "UART_RXD1", Pin::Start_up, Pin::Port_A, 21, Pin::Mode_A, Pin::Input_normal}, // 32
            {Tag::Pin_UART_TXD1, "UART_TXD1", Pin::Start_up, Pin::Port_B, 4, Pin::Mode_D, Pin::Out_normal},    // 105
            {Tag::Pin_LED0, "LED0", Pin::Start_up, Pin::Port_A, 23, Pin::Mode_GPIO, Pin::Out_normal},          // 46
            {Tag::Pin_LED1, "LED1", Pin::Start_up, Pin::Port_C, 9, Pin::Mode_GPIO, Pin::Out_normal},           // 86
            {Tag::Pin_ALT_WKUP6},
            {Tag::Pin_TEMP_ALERT},

        };
    };
}
