#pragma once

#include "IO_pins.h"
#include "Dictionary/Error_codes.h"

namespace SamV71_pin_manager {
    using namespace Application_configuration;
    using Pin = Abstract::Abstract_IO_pin;

    [[nodiscard]] Error_codes::Error_code initialize(Pin::Pin_phase);

    Pin &get_pin(Pin_id);

    Pin *get_optional_pin(Pin_id);

    void print_diagnostics();

};
