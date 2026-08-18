#pragma once

#include "Interface/IO_pin_interface.h"
#include "component/gpio/pio.h"

namespace SamV71 {

    class SamV71_IO_pin : public Interface::IO_pin_interface {
    public:
        SamV71_IO_pin() = delete;

        SamV71_IO_pin(uint8_t id) : the_name("nu"), optional_base(nullptr), the_pin_mask(0), the_id(id) {}

        SamV71_IO_pin(uint8_t id, const char *name, Pin_phase phase, Pin_port port, uint8_t pin,
                       Pin_mode mux, Pin_type type, uint8_t strength = 0, bool default_state = false);

        [[nodiscard]] RMU_error_codes::Error_code initialize(uint8_t id, Pin_phase) override;

        void neutralize() override;

        uint8_t get_id() const override { return the_id; }

        bool get() const override;

        void set() override;

        void clear() override;

        void toggle() override;

        void pulse() override;

        void pulse(uint32_t count) override;

        void print_diagnostics() const override;

        bool is_used() const override { return optional_base != nullptr; }

        void report(Service_report &report, RMU_interface::Keys as) const final;

        void set_GPIO_mode() final;

        void restore_peripheral_function() final;

        static void initialize_clocks();

    private:
        void reinitialize(Pin_mode mode);

        uint32_t get_new_config(Pin_mode mux, Pin_type type, uint8_t strength);

        uint32_t get_current_config() const;

        void configure_pin(uint32_t config) const;

        bool is_GPIO() const { return (the_mode == 0); };

    private:
        const char *the_name;
        pio_group_registers_t *const optional_base;
        const uint32_t the_pin_mask;
        const uint8_t the_id;
        const Pin_phase the_phase{Never_up};
        const Pin_port the_port{};
        const uint8_t the_pin{};
        const Pin_mode the_mode{};
        Pin_mode the_current_mode{};
        const Pin_type the_type{};
        const uint8_t the_strength{};
        const bool the_default_state{};
    };

}
