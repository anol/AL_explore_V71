#pragma once

#include <cstdint>

#include "Error_codes.h"

namespace Abstract {

    /// Purpose: The Abstract_IO_pin is a hardware abstraction of the MCU peripheral IOs.
    class Abstract_IO_pin {
    public:

        /// Purpose: The Pin_phase is used indicate when a pin is initialized
        enum Pin_phase : uint8_t {
            Start_up, Host_up, Board_up, Detector_up, Never_up,
        };

        /// Purpose: The Pin_port is used for pin addressing
        enum Pin_port : uint8_t {
            Port_A, Port_B, Port_C, Port_D, Port_E,
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
        [[nodiscard]] virtual Error_codes::Error_code initialize(uint8_t id, Pin_phase) = 0;

        [[nodiscard]] virtual uint8_t get_id() const = 0;

        [[nodiscard]] virtual bool get() const = 0;

        virtual void set() = 0;

        virtual void clear() = 0;

        virtual void toggle() = 0;

        virtual void pulse() = 0;

        virtual void pulse(uint32_t count) = 0;

        [[nodiscard]] virtual bool is_used() const = 0;
    };

} // Interface

#endif //INTERFACE_IO_PIN_INTERFACE_H
