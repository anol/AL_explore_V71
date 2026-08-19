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
    class Abstract_IO_pin {
    public:


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
