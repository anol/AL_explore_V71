

#include <stdio.h>

#include "SamV71_IO_pin.h"
#include "V71_EK_pin_table.h"
#include "SamV71_pin_manager.h"


    namespace SamV71_pin_manager {
        using Pin = Abstract::Abstract_IO_pin;
        static Configuration::V71_EK_pin_table<SamV71::SamV71_IO_pin> the_pin_table;

        Error_codes::Error_code initialize(Pin::Pin_phase phase) {
            if ((phase == Pin::Start_up) || (phase == Pin::Host_up)) {
                SamV71::SamV71_IO_pin::initialize_clocks();
            }
            uint8_t id_counter{};
            for (auto &pin: the_pin_table.the_pins) {
                auto result = pin.initialize(id_counter++, phase);
                if (result.failed()) {
                    return result;
                }
            }
            return {};
        }

        Pin &get_pin(Pin::Pin_id id) {
            if (id < Pin::Number_of_pins) {
                return the_pin_table.the_pins[id];
            } else {
                return the_pin_table.the_pins[Pin::Pin_not_used];
            }
        }

        Pin *get_optional_pin(Pin::Pin_id id) {
            if (id < Pin::Number_of_pins) {
                return &the_pin_table.the_pins[id];
            } else {
                return {};
            }
        }

        void print_diagnostics() {
            printf("NORM_pin_manager\r\n");
            for (auto &pin: the_pin_table.the_pins) {
                if (pin.is_used()) {
                    pin.print_diagnostics();
                }
            }
        }


    } // NORM_pin_manager
