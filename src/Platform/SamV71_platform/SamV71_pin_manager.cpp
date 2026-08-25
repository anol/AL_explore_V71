#include <stdio.h>

#include "SamV71_IO_pin.h"
#include "V71_EK_pin_table.h"
#include "SamV71_pin_manager.h"


namespace SamV71_pin_manager {
    using namespace Application_configuration;
    using Pin = Abstract::Abstract_IO_pin;
    static Configuration::V71_EK_pin_table<SamV71::SamV71_IO_pin> the_pin_table;

    bool initialize(Pin::Pin_phase phase) {
        if ((phase == Pin::Start_up) || (phase == Pin::Host_up)) {
            SamV71::SamV71_IO_pin::initialize_clocks();
        }
        uint8_t id_counter{};
        for (auto &pin: the_pin_table.the_pins) {
            auto result = pin.initialize(id_counter++, phase);
            return result.success();
        }
        return false;
    }

    Pin &get_pin(const Pin_id id) {
        if (id < Number_of_pins) {
            return the_pin_table.the_pins[id];
        }
        return the_pin_table.the_pins[Pin_not_used];
    }

    Pin *get_optional_pin(const Pin_id id) {
        if (id < Number_of_pins) {
            return &the_pin_table.the_pins[id];
        }
        return {};
    }

    void print_diagnostics() {
        printf("NORM_pin_manager\r\n");
        // for (auto& pin : the_pin_table.the_pins)
        // {
        //     if (pin.is_used())
        //     {
        //         pin.print_diagnostics();
        //     }
        // }
    }
} // NORM_pin_manager
