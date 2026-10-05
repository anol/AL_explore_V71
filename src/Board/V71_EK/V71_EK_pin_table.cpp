module;
#include <cstdint>
#include <cstdio>
#include "sam.h"
#include "component/matrix.h"

module Board.V71_EK_pin_table;
import Platform.SamV71_IO_pin;
import Domain.IO_pins;
import Type.Abstract_IO_pin;
import Type.Abstract_pin_table;
import Platform.SamV71_clock;

namespace SamV71 {
    using Pin = Abstract::Abstract_IO_pin;

    void V71_EK_pin_table::initialize() {
        set_phase(Abstract::Abstract_IO_pin::Start_up);
    }

    void V71_EK_pin_table::enable_peripheral_clocks() {
        SamV71_clock::enable_peripheral_clock(ID_PIOA);
        SamV71_clock::enable_peripheral_clock(ID_PIOB);
        SamV71_clock::enable_peripheral_clock(ID_PIOC);
        SamV71_clock::enable_peripheral_clock(ID_PIOD);
        SamV71_clock::enable_peripheral_clock(ID_PIOE);
    }

    Status_code V71_EK_pin_table::set_phase(Pin::Pin_phase phase) {
        uint8_t id_counter{};
        for (auto &pin: the_pin_table) {
            if (pin.initialize(id_counter++, phase).failed()) {
                return Status_code::Failure();
            }
        }
        return Status_code::Success();
    }

    Status_code V71_EK_pin_table::for_each_pin(void *user, void (*func)(void *, Abstract::Abstract_IO_pin &)) {
        for (auto &pin: the_pin_table) {
            if (func) { func(user, pin); }
        }
        return Status_code::Success();
    }

    Pin &V71_EK_pin_table::get_pin(const Abstract::Abstract_pin_id id) {
        if (id < Domain::Number_of_pins) {
            return the_pin_table[id];
        }
        return the_pin_table[Domain::Pin_not_used];
    }

    void V71_EK_pin_table::print_diagnostics() const {
        printf("Pin manager\r\n");
        for (auto &pin: the_pin_table) {
            if (pin.is_used()) {
                pin.print_diagnostics();
            }
        }
    }
}
