/*
 * Copyright (C) 2024 Integrated Detector Electronics AS
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
 * \date   IDEAS/2024.08.13/aeols
 * \brief  NORM Board Support
 */

#ifndef CONFIGURATION_NORM_PIN_TABLE_H
#define CONFIGURATION_NORM_PIN_TABLE_H

#include "Abstract_IO_pin.h"

namespace Configuration {
    template<typename IO_pin>
    class V71_EK_pin_table {
        using Pin = Abstract_IO_pin;
        using Type = Pin::Pin_type;
        using Phase = Pin::Pin_phase;

    public:
        IO_pin the_pins[Pin::Number_of_pins]{
                // Pin name,       Function,                Phase,              Port,      Pin,  Mode,           Type,          Strength=0,  Default state=low
                {Pin::Pin_not_used},
                {Pin::Pin_1553B_TXIHB,    "1553B_TXIHB",   Phase::Host_up,     Pin::Port_F, 21, Pin::Mode_GPIO, Pin::Out_open_drain},
                {Pin::Pin_CAN_LOOPBK0,    "CAN_LOOPBK0",   Phase::Host_up,     Pin::Port_B, 2,  Pin::Mode_GPIO, Pin::Out_normal},
                {Pin::Pin_CAN_LOOPBK1},
                {Pin::Pin_CAN_RX0,        "CAN_RX0",       Phase::Host_up,     Pin::Port_B, 7,  Pin::Mode_C,    Pin::Input_normal},
                {Pin::Pin_CAN_RX1,        "CAN_RX1",       Phase::Host_up,     Pin::Port_B, 5,  Pin::Mode_C,    Pin::Input_normal},
                {Pin::Pin_CAN_STBY0,      "CAN_STBY0",     Phase::Host_up,     Pin::Port_B, 0,  Pin::Mode_GPIO, Pin::Out_normal},
                {Pin::Pin_CAN_STBY1},
                {Pin::Pin_CAN_TX0,        "CAN_TX0",       Phase::Host_up,     Pin::Port_B, 6,  Pin::Mode_C,    Pin::Out_normal, 1},
                {Pin::Pin_CAN_TX1,        "CAN_TX1",       Phase::Host_up,     Pin::Port_B, 4,  Pin::Mode_C,    Pin::Out_normal, 1},
                {Pin::Pin_DetBias_Clk,    "DetBias_Clk",   Phase::Detector_up, Pin::Port_C, 18, Pin::Mode_A,    Pin::Out_normal, 4},
                {Pin::Pin_DGU_bias,       "DGU_bias",      Phase::Detector_up, Pin::Port_B, 26, Pin::Mode_GPIO, Pin::Out_normal, 2},
                {Pin::Pin_DGU_DCAL,       "DGU_DCAL",      Phase::Detector_up, Pin::Port_B, 9,  Pin::Mode_A,    Pin::Out_normal, 3},
                {Pin::Pin_DGU_DRESET,     "DGU_DRESET",    Phase::Detector_up, Pin::Port_B, 20, Pin::Mode_GPIO, Pin::Out_normal, 2},
                {Pin::Pin_DGU_DTVO_BUF},
                {Pin::Pin_DGU_ERR,        "DGU_ERR",       Phase::Detector_up, Pin::Port_B, 11, Pin::Mode_GPIO, Pin::Input_pull_down},
                {Pin::Pin_DGU_LU1},
                {Pin::Pin_DGU_LU2},
                {Pin::Pin_DGU_MSTEST},
                {Pin::Pin_DGU_Power_ENb,  "DGU_Power_ENb", Phase::Start_up,    Pin::Port_B, 29, Pin::Mode_GPIO, Pin::Out_normal, 0},
                {Pin::Pin_DGU_RO_CLK,     "DGU_RO_CLK",    Phase::Detector_up, Pin::Port_B, 17, Pin::Mode_GPIO, Pin::Out_normal, 3},
                {Pin::Pin_DGU_SHIFT_IN,   "DGU_SHIFT_IN",  Phase::Detector_up, Pin::Port_B, 18, Pin::Mode_GPIO, Pin::Out_normal, 3},
                {Pin::Pin_DGU_SHIFT_OUT},
                {Pin::Pin_DGU_SPI_CS0,    "DGU_SPI_CS0",   Phase::Detector_up, Pin::Port_C, 3,  Pin::Mode_A,    Pin::Out_normal, 1},
                {Pin::Pin_DGU_SPI_CS1,    "DGU_SPI_CS1",   Phase::Detector_up, Pin::Port_C, 4,  Pin::Mode_A,    Pin::Out_normal, 1},
                {Pin::Pin_DGU_SPI_CS2},
                {Pin::Pin_DGU_SPI_MISO,   "DGU_SPI_MISO",  Phase::Detector_up, Pin::Port_C, 1,  Pin::Mode_A,    Pin::Input_pull_down},
                {Pin::Pin_DGU_SPI_MOSI,   "DGU_SPI_MOSI",  Phase::Detector_up, Pin::Port_C, 0,  Pin::Mode_A,    Pin::Out_normal, 1},
                {Pin::Pin_DGU_SPI_SCK,    "DGU_SPI_SCK",   Phase::Detector_up, Pin::Port_C, 2,  Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Pin_DGU_SRESET,     "DGU_SRESET",    Phase::Detector_up, Pin::Port_C, 5,  Pin::Mode_GPIO, Pin::Out_normal, 2},
                {Pin::Pin_DGU_SS_HOLD,    "DGU_SS_HOLD",   Phase::Detector_up, Pin::Port_B, 12, Pin::Mode_A,    Pin::Out_normal, 3},
                {Pin::Pin_DGU_THGVO_1,    "DGU_THGVO_1",   Phase::Detector_up, Pin::Port_B, 13, Pin::Mode_A,    Pin::Input_pull_down},
                {Pin::Pin_DGU_THGVO_2},
                {Pin::Pin_DGU_TLGVO},
                {Pin::Pin_DOSI_MUX_A0},
                {Pin::Pin_DOSI_MUX_A1},
                {Pin::Pin_DOSI_MUX_A2},
                {Pin::Pin_DOSI_MUX_EN},
                {Pin::Pin_DOSI_RO_EN},
                {Pin::Pin_I2C_CK,         "I2C_CK",        Phase::Board_up,    Pin::Port_C, 22, Pin::Mode_A,    Pin::Out_normal, 1},
                {Pin::Pin_I2C_D,          "I2C_D",         Phase::Board_up,    Pin::Port_C, 21, Pin::Mode_A,    Pin::Out_normal, 1},
                {Pin::Pin_MCU_NVM_NRESET, "NVM_NRESET",    Phase::Start_up,    Pin::Port_G, 30, Pin::Mode_GPIO, Pin::Out_normal, 1},
                {Pin::Pin_NWDT0},
                {Pin::Pin_PIO_2_alarm,    "PIO_2_alarm",   Phase::Start_up,    Pin::Port_C, 23, Pin::Mode_GPIO, Pin::Out_normal, 3},
                {Pin::Pin_PIO_1,          "PIO_1",         Phase::Start_up,    Pin::Port_B, 23, Pin::Mode_GPIO, Pin::Out_normal, 3},
                {Pin::Pin_TempS_THERMb,   "TempS_THERMb",  Phase::Board_up,    Pin::Port_C, 24, Pin::Mode_GPIO, Pin::Input_normal},
                {Pin::Pin_UART_Rx,        "UART_Rx",       Phase::Start_up,    Pin::Port_F, 29, Pin::Mode_A,    Pin::Input_pull_up},
                {Pin::Pin_UART_Tx,        "UART_Tx",       Phase::Start_up,    Pin::Port_F, 30, Pin::Mode_A,    Pin::Out_normal, 1},
                {Pin::Xio_ADDR_0,         "A0",            Phase::Start_up,    Pin::Port_G, 0,  Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_1,         "A1",            Phase::Start_up,    Pin::Port_G, 1,  Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_2,         "A2",            Phase::Start_up,    Pin::Port_G, 2,  Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_3,         "A3",            Phase::Start_up,    Pin::Port_G, 3,  Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_4,         "A4",            Phase::Start_up,    Pin::Port_G, 4,  Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_5,         "A5",            Phase::Start_up,    Pin::Port_G, 5,  Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_6,         "A6",            Phase::Start_up,    Pin::Port_G, 6,  Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_7,         "A7",            Phase::Start_up,    Pin::Port_G, 7,  Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_8,         "A8",            Phase::Start_up,    Pin::Port_G, 8,  Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_9,         "A9",            Phase::Start_up,    Pin::Port_G, 9,  Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_10,        "A10",           Phase::Start_up,    Pin::Port_G, 10, Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_11,        "A11",           Phase::Start_up,    Pin::Port_G, 11, Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_12,        "A12",           Phase::Start_up,    Pin::Port_G, 12, Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_13,        "A13",           Phase::Start_up,    Pin::Port_G, 13, Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_14,        "A14",           Phase::Start_up,    Pin::Port_G, 14, Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_15,        "A15",           Phase::Start_up,    Pin::Port_G, 15, Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_16,        "A16",           Phase::Start_up,    Pin::Port_G, 16, Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_17,        "A17",           Phase::Start_up,    Pin::Port_G, 17, Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_18,        "A18",           Phase::Start_up,    Pin::Port_G, 18, Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_19,        "A19",           Phase::Start_up,    Pin::Port_G, 19, Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_ADDR_20,        "A20",           Phase::Start_up,    Pin::Port_G, 20, Pin::Mode_A,    Pin::Out_normal, 2},
                {Pin::Xio_CB_0,           "CB0",           Phase::Start_up,    Pin::Port_E, 0,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_1,           "CB1",           Phase::Start_up,    Pin::Port_E, 1,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_2,           "CB2",           Phase::Start_up,    Pin::Port_E, 2,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_3,           "CB3",           Phase::Start_up,    Pin::Port_E, 3,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_4,           "CB4",           Phase::Start_up,    Pin::Port_E, 4,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_5,           "CB5",           Phase::Start_up,    Pin::Port_E, 5,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_6,           "CB6",           Phase::Start_up,    Pin::Port_E, 6,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_7,           "CB7",           Phase::Start_up,    Pin::Port_E, 7,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_8,           "CB8",           Phase::Start_up,    Pin::Port_E, 8,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_9,           "CB9",           Phase::Start_up,    Pin::Port_E, 9,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_10,          "CB10",          Phase::Start_up,    Pin::Port_E, 10, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_11,          "CB11",          Phase::Start_up,    Pin::Port_E, 11, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_12,          "CB12",          Phase::Start_up,    Pin::Port_E, 12, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_13,          "CB13",          Phase::Start_up,    Pin::Port_C, 31, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_14,          "CB14",          Phase::Start_up,    Pin::Port_C, 30, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_CB_15,          "CB15",          Phase::Start_up,    Pin::Port_C, 29, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_0,         "D0",            Phase::Start_up,    Pin::Port_D, 0,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_1,         "D1",            Phase::Start_up,    Pin::Port_D, 1,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_2,         "D2",            Phase::Start_up,    Pin::Port_D, 2,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_3,         "D3",            Phase::Start_up,    Pin::Port_D, 3,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_4,         "D4",            Phase::Start_up,    Pin::Port_D, 4,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_5,         "D5",            Phase::Start_up,    Pin::Port_D, 5,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_6,         "D6",            Phase::Start_up,    Pin::Port_D, 6,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_7,         "D7",            Phase::Start_up,    Pin::Port_D, 7,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_8,         "D8",            Phase::Start_up,    Pin::Port_D, 8,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_9,         "D9",            Phase::Start_up,    Pin::Port_D, 9,  Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_10,        "D10",           Phase::Start_up,    Pin::Port_D, 10, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_11,        "D11",           Phase::Start_up,    Pin::Port_D, 11, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_12,        "D12",           Phase::Start_up,    Pin::Port_D, 12, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_13,        "D13",           Phase::Start_up,    Pin::Port_D, 13, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_14,        "D14",           Phase::Start_up,    Pin::Port_D, 14, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_15,        "D15",           Phase::Start_up,    Pin::Port_D, 15, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_16,        "D16",           Phase::Start_up,    Pin::Port_D, 16, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_17,        "D17",           Phase::Start_up,    Pin::Port_D, 17, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_18,        "D18",           Phase::Start_up,    Pin::Port_D, 18, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_19,        "D19",           Phase::Start_up,    Pin::Port_D, 19, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_20,        "D20",           Phase::Start_up,    Pin::Port_D, 20, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_21,        "D21",           Phase::Start_up,    Pin::Port_D, 21, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_22,        "D22",           Phase::Start_up,    Pin::Port_D, 22, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_23,        "D23",           Phase::Start_up,    Pin::Port_D, 23, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_24,        "D24",           Phase::Start_up,    Pin::Port_D, 24, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_25,        "D25",           Phase::Start_up,    Pin::Port_D, 25, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_26,        "D26",           Phase::Start_up,    Pin::Port_D, 26, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_27,        "D27",           Phase::Start_up,    Pin::Port_D, 27, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_28,        "D28",           Phase::Start_up,    Pin::Port_D, 28, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_29,        "D29",           Phase::Start_up,    Pin::Port_D, 29, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_30,        "D30",           Phase::Start_up,    Pin::Port_D, 30, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_DATA_31,        "D31",           Phase::Start_up,    Pin::Port_D, 31, Pin::Mode_A,    Pin::Input_normal},
                {Pin::Xio_NCS_0,          "NCS_0",         Phase::Start_up,    Pin::Port_F, 11, Pin::Mode_A,    Pin::Out_normal},
                {Pin::Xio_NCS_1,          "NCS_1",         Phase::Start_up,    Pin::Port_F, 12, Pin::Mode_A,    Pin::Out_normal},
                {Pin::Xio_NCS_2,          "NCS_2",         Phase::Start_up,    Pin::Port_F, 13, Pin::Mode_A,    Pin::Out_normal},
                {Pin::Xio_NRD,            "NRD",           Phase::Start_up,    Pin::Port_F, 17, Pin::Mode_A,    Pin::Out_normal, 4},
                {Pin::Xio_NWE,            "NWE",           Phase::Start_up,    Pin::Port_F, 18, Pin::Mode_A,    Pin::Out_normal, 4},

        };

    };
}
#endif //CONFIGURATION_NORM_PIN_TABLE_H
