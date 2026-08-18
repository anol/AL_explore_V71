

#include <stdio.h>

#include "SamV71_IO_pin.h"
#include "Configuration/SamV71_pin_table.h"

#include "SamV71_pin_manager.h"


    namespace SamV71_pin_manager {
        using Pin = Interface::IO_pin_interface;
        Configuration::NORM_EM_pin_table<SAMRH71::SAMRH71_IO_pin> the_pin_table;

        RMU_error_codes::Error_code initialize(Pin::Pin_phase phase) {
            if ((phase == Interface::IO_pin_interface::Start_up) || (phase == Interface::IO_pin_interface::Host_up)) {
                SAMRH71::SAMRH71_IO_pin::initialize_clocks();
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


        void report_all(Service_report &report)
        {
            for (auto &pin: the_pin_table.the_pins)
            {
                if (pin.is_used())
                {
                    pin.report(report, RMU_interface::No_key);
                }
            }
        }


        void report(Service_report &report, uint8_t id)
        {
            for (auto &pin: the_pin_table.the_pins)
            {
                if (pin.get_id() == id)
                {
                    pin.report(report, RMU_interface::Key_info);
                    break;
                }
            }
        }

    } // NORM_pin_manager
