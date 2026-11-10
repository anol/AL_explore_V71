module;
#include <cstdint>
#include <cstdio>

module Board.Win11_pin_table;

namespace Board {
    namespace {
        struct Pin_info {
            Domain::Pin_id the_id;
            const char *the_name;
            bool the_default_state;
        };

        constexpr Pin_info Named_pins[] = {
            {Domain::Pin_SPI_CS_ASIC1, "CS_ASIC1", true},
            {Domain::Pin_SPI_CS_ASIC2, "CS_ASIC2", true},
            {Domain::Pin_SPI_CS_ASIC3, "CS_ASIC3", true},
            {Domain::Pin_SPI_CS_ASIC4, "CS_ASIC4", true},
            {Domain::Pin_SPI_CS_ASIC5, "CS_ASIC5", true},
            {Domain::Pin_SPI_CS_DAC0, "CS_DAC0", true},
            {Domain::Pin_SPI_CS_DAC1, "CS_DAC1", true},
            {Domain::Pin_FPGA_CS_N, "CS_FPGA", true},
            {Domain::Pin_LED0, "LED0", false},
            {Domain::Pin_LED1, "LED1", false},
            {Domain::Pin_ASIC_RESET_I, "ASIC_RESET", false},
            {Domain::Pin_ASIC_HOLD_I, "ASIC_HOLD", false},
        };
    }

    Win11_pin_table::Win11_pin_table() {
        for (uint8_t id = 0; id < Domain::Number_of_pins; id++) {
            the_pin_table[id].configure(id, "nu", false);
        }
        for (const auto &info: Named_pins) {
            the_pin_table[info.the_id].configure(info.the_id, info.the_name, info.the_default_state);
        }
    }

    void Win11_pin_table::initialize() {
        for (const auto &info: Named_pins) {
            (void) the_pin_table[info.the_id].initialize(info.the_id, Abstract::Abstract_IO_pin::Start_up);
        }
    }

    Abstract::Abstract_IO_pin &Win11_pin_table::get_pin(const Abstract::Abstract_pin_id id) {
        return the_pin_table[id < Domain::Number_of_pins ? id : static_cast<Abstract::Abstract_pin_id>(Domain::Pin_not_used)];
    }

    Status_code Win11_pin_table::for_each_pin(void *user, void (*func)(void *, Abstract::Abstract_IO_pin &)) {
        if (func) {
            for (auto &pin: the_pin_table) {
                func(user, pin);
            }
        }
        return Status_code::Success();
    }

    void Win11_pin_table::print_diagnostics() const {
        printf("Pins (mockup):\r\n");
        for (const auto &pin: the_pin_table) {
            if (pin.is_used()) {
                pin.print_diagnostics();
            }
        }
    }
} // Board
