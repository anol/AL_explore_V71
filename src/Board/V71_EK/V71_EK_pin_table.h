#pragma once
#include "IO_pins.h"

namespace Configuration {
    template<typename IO_pin>
    class V71_EK_pin_table {
        using Tag = Application_configuration::Pin_id;
        using Pin = Abstract::Abstract_IO_pin;

    public:
        IO_pin the_pins[Tag::Number_of_pins]{
                // Pin name,       Function,                Phase,              Port,      Pin,  Mode,           Type,          Strength=0,  Default state=low
                {Tag::Pin_not_used},
                {Tag::Pin_CAN_LOOPBK0,    "CAN_LOOPBK0",   Pin::Host_up,     Pin::Port_B, 2,  Pin::Mode_GPIO, Pin::Out_normal},
                {Tag::Pin_CAN_LOOPBK1},
                {Tag::Pin_CAN_RX0,        "CAN_RX0",       Pin::Host_up,     Pin::Port_B, 7,  Pin::Mode_C,    Pin::Input_normal},
                {Tag::Pin_CAN_RX1,        "CAN_RX1",       Pin::Host_up,     Pin::Port_B, 5,  Pin::Mode_C,    Pin::Input_normal},
                {Tag::Pin_CAN_STBY0,      "CAN_STBY0",     Pin::Host_up,     Pin::Port_B, 0,  Pin::Mode_GPIO, Pin::Out_normal},
                {Tag::Pin_CAN_STBY1},
                {Tag::Pin_CAN_TX0,        "CAN_TX0",       Pin::Host_up,     Pin::Port_B, 6,  Pin::Mode_C,    Pin::Out_normal, 1},
                {Tag::Pin_CAN_TX1,        "CAN_TX1",       Pin::Host_up,     Pin::Port_B, 4,  Pin::Mode_C,    Pin::Out_normal, 1},
                {Tag::Pin_DetBias_Clk,    "DetBias_Clk",   Pin::Detector_up, Pin::Port_C, 18, Pin::Mode_A,    Pin::Out_normal, 4},
                {Tag::Pin_DGU_bias,       "DGU_bias",      Pin::Detector_up, Pin::Port_B, 26, Pin::Mode_GPIO, Pin::Out_normal, 2},
                {Tag::Pin_DGU_DCAL,       "DGU_DCAL",      Pin::Detector_up, Pin::Port_B, 9,  Pin::Mode_A,    Pin::Out_normal, 3},
                {Tag::Pin_DGU_DRESET,     "DGU_DRESET",    Pin::Detector_up, Pin::Port_B, 20, Pin::Mode_GPIO, Pin::Out_normal, 2},
                {Tag::Pin_DGU_DTVO_BUF},
                {Tag::Pin_DGU_ERR,        "DGU_ERR",       Pin::Detector_up, Pin::Port_B, 11, Pin::Mode_GPIO, Pin::Input_pull_down},
                {Tag::Pin_DGU_LU1},
                {Tag::Pin_DGU_LU2},
                {Tag::Pin_DGU_MSTEST},
                {Tag::Pin_DGU_Power_ENb,  "DGU_Power_ENb", Pin::Start_up,    Pin::Port_B, 29, Pin::Mode_GPIO, Pin::Out_normal, 0},
                {Tag::Pin_DGU_RO_CLK,     "DGU_RO_CLK",    Pin::Detector_up, Pin::Port_B, 17, Pin::Mode_GPIO, Pin::Out_normal, 3},
                {Tag::Pin_DGU_SHIFT_IN,   "DGU_SHIFT_IN",  Pin::Detector_up, Pin::Port_B, 18, Pin::Mode_GPIO, Pin::Out_normal, 3},
                {Tag::Pin_DGU_SHIFT_OUT},
                {Tag::Pin_DGU_SPI_CS0,    "DGU_SPI_CS0",   Pin::Detector_up, Pin::Port_C, 3,  Pin::Mode_A,    Pin::Out_normal, 1},
                {Tag::Pin_DGU_SPI_CS1,    "DGU_SPI_CS1",   Pin::Detector_up, Pin::Port_C, 4,  Pin::Mode_A,    Pin::Out_normal, 1},
                {Tag::Pin_DGU_SPI_CS2},
                {Tag::Pin_DGU_SPI_MISO,   "DGU_SPI_MISO",  Pin::Detector_up, Pin::Port_C, 1,  Pin::Mode_A,    Pin::Input_pull_down},
                {Tag::Pin_DGU_SPI_MOSI,   "DGU_SPI_MOSI",  Pin::Detector_up, Pin::Port_C, 0,  Pin::Mode_A,    Pin::Out_normal, 1},
                {Tag::Pin_DGU_SPI_SCK,    "DGU_SPI_SCK",   Pin::Detector_up, Pin::Port_C, 2,  Pin::Mode_A,    Pin::Out_normal, 2},
                {Tag::Pin_DGU_SRESET,     "DGU_SRESET",    Pin::Detector_up, Pin::Port_C, 5,  Pin::Mode_GPIO, Pin::Out_normal, 2},
                {Tag::Pin_DGU_SS_HOLD,    "DGU_SS_HOLD",   Pin::Detector_up, Pin::Port_B, 12, Pin::Mode_A,    Pin::Out_normal, 3},
                {Tag::Pin_DGU_THGVO_1,    "DGU_THGVO_1",   Pin::Detector_up, Pin::Port_B, 13, Pin::Mode_A,    Pin::Input_pull_down},
                {Tag::Pin_DGU_THGVO_2},
                {Tag::Pin_DGU_TLGVO},
                {Tag::Pin_DOSI_MUX_A0},
                {Tag::Pin_DOSI_MUX_A1},
                {Tag::Pin_DOSI_MUX_A2},
                {Tag::Pin_DOSI_MUX_EN},
                {Tag::Pin_DOSI_RO_EN},
                {Tag::Pin_I2C_CK,         "I2C_CK",        Pin::Board_up,    Pin::Port_C, 22, Pin::Mode_A,    Pin::Out_normal, 1},
                {Tag::Pin_I2C_D,          "I2C_D",         Pin::Board_up,    Pin::Port_C, 21, Pin::Mode_A,    Pin::Out_normal, 1},
            };

    };
}
