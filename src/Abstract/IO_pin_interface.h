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
 * \date   IDEAS/2024.08.12/aeols
 * \brief  Board Support wrt. IO pins
 */

#ifndef INTERFACE_IO_PIN_INTERFACE_H
#define INTERFACE_IO_PIN_INTERFACE_H

#include <cstdint>
#include "Dictionary/Dictionary.h"
#include "Plumbing/Transaction/Service_report.h"

namespace Interface {

    /// Purpose: The IO_pin_interface is a hardware abstraction of the MCU peripheral IOs.
    class IO_pin_interface {
    public:

        /// Purpose: The Pin_id specifies the function of a pin
        enum Pin_id : uint8_t {
            Pin_not_used,

            Pin_1553B_TXIHB,
            Pin_CAN_LOOPBK0,
            Pin_CAN_LOOPBK1,
            Pin_CAN_RX0,
            Pin_CAN_RX1,
            Pin_CAN_STBY0,
            Pin_CAN_STBY1,
            Pin_CAN_TX0,
            Pin_CAN_TX1,
            Pin_DetBias_Clk,
            Pin_DGU_bias,
            Pin_DGU_DCAL,
            Pin_DGU_DRESET,
            Pin_DGU_DTVO_BUF,
            Pin_DGU_ERR,
            Pin_DGU_LU1,
            Pin_DGU_LU2,
            Pin_DGU_MSTEST,
            Pin_DGU_Power_ENb,
            Pin_DGU_RO_CLK,
            Pin_DGU_SHIFT_IN,
            Pin_DGU_SHIFT_OUT,
            Pin_DGU_SPI_CS0,
            Pin_DGU_SPI_CS1,
            Pin_DGU_SPI_CS2,
            Pin_DGU_SPI_MISO,
            Pin_DGU_SPI_MOSI,
            Pin_DGU_SPI_SCK,
            Pin_DGU_SRESET,
            Pin_DGU_SS_HOLD,
            Pin_DGU_THGVO_1,
            Pin_DGU_THGVO_2,
            Pin_DGU_TLGVO,
            Pin_DOSI_MUX_A0,
            Pin_DOSI_MUX_A1,
            Pin_DOSI_MUX_A2,
            Pin_DOSI_MUX_EN,
            Pin_DOSI_RO_EN,
            Pin_I2C_CK,
            Pin_I2C_D,
            Pin_MCU_NVM_NRESET,
            Pin_NWDT0,
            Pin_PIO_2_alarm,
            Pin_PIO_1,
            Pin_TempS_THERMb,
            Pin_UART_Rx,
            Pin_UART_Tx,
            Xio_ADDR_0,
            Xio_ADDR_1,
            Xio_ADDR_2,
            Xio_ADDR_3,
            Xio_ADDR_4,
            Xio_ADDR_5,
            Xio_ADDR_6,
            Xio_ADDR_7,
            Xio_ADDR_8,
            Xio_ADDR_9,
            Xio_ADDR_10,
            Xio_ADDR_11,
            Xio_ADDR_12,
            Xio_ADDR_13,
            Xio_ADDR_14,
            Xio_ADDR_15,
            Xio_ADDR_16,
            Xio_ADDR_17,
            Xio_ADDR_18,
            Xio_ADDR_19,
            Xio_ADDR_20,
            Xio_CB_0,
            Xio_CB_1,
            Xio_CB_2,
            Xio_CB_3,
            Xio_CB_4,
            Xio_CB_5,
            Xio_CB_6,
            Xio_CB_7,
            Xio_CB_8,
            Xio_CB_9,
            Xio_CB_10,
            Xio_CB_11,
            Xio_CB_12,
            Xio_CB_13,
            Xio_CB_14,
            Xio_CB_15,
            Xio_DATA_0,
            Xio_DATA_1,
            Xio_DATA_2,
            Xio_DATA_3,
            Xio_DATA_4,
            Xio_DATA_5,
            Xio_DATA_6,
            Xio_DATA_7,
            Xio_DATA_8,
            Xio_DATA_9,
            Xio_DATA_10,
            Xio_DATA_11,
            Xio_DATA_12,
            Xio_DATA_13,
            Xio_DATA_14,
            Xio_DATA_15,
            Xio_DATA_16,
            Xio_DATA_17,
            Xio_DATA_18,
            Xio_DATA_19,
            Xio_DATA_20,
            Xio_DATA_21,
            Xio_DATA_22,
            Xio_DATA_23,
            Xio_DATA_24,
            Xio_DATA_25,
            Xio_DATA_26,
            Xio_DATA_27,
            Xio_DATA_28,
            Xio_DATA_29,
            Xio_DATA_30,
            Xio_DATA_31,
            Xio_NCS_0,
            Xio_NCS_1,
            Xio_NCS_2,
            Xio_NRD,
            Xio_NWE,

            Number_of_pins
        };

        /// Purpose: The Pin_phase is used indicate when a pin is initialized
        enum Pin_phase : uint8_t {
            Start_up, Host_up, Board_up, Detector_up, Never_up,
        };

        /// Purpose: The Pin_port is used for pin addressing
        enum Pin_port : uint8_t {
            Port_A, Port_B, Port_C, Port_D, Port_E, Port_F, Port_G
        };

        /// Purpose: The Pin_mux setting is used to select which peripheral is controlling the pin
        enum Pin_mode : uint8_t {
            Mode_GPIO, Mode_A, Mode_B, Mode_C, Mode_D,
        };

        /// Purpose: The Pin_type specifies the pin electric configuration
        enum Pin_type : uint8_t {
            Input_normal, Input_pull_up, Input_pull_down, Input_Schmitt_trigger,
            Out_normal, Out_open_drain, Out_open_drain_pull_up,
        };

    public:
        [[nodiscard]] virtual RMU_error_codes::Error_code initialize(uint8_t id, Pin_phase) = 0;

        virtual void neutralize() = 0;

        virtual uint8_t get_id() const = 0;

        virtual bool get() const = 0;

        virtual void set() = 0;

        virtual void clear() = 0;

        virtual void toggle() = 0;

        virtual void pulse() = 0;

        virtual void pulse(uint32_t count) = 0;

        virtual void print_diagnostics() const = 0;

        virtual bool is_used() const = 0;

        /// @note as can be No_key
        virtual void report(Service_report &report, RMU_interface::Keys as) const = 0;

        virtual void set_GPIO_mode() = 0;

        virtual void restore_peripheral_function() = 0;
    };

} // Interface

#endif //INTERFACE_IO_PIN_INTERFACE_H
