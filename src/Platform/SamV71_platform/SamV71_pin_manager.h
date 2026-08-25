#pragma once

#include "Abstract_IO_pin.h"
#include "IO_pins.h"
#include "Dictionary/Error_codes.h"

namespace SamV71 {
    using namespace Application_configuration;

    class SamV71_pin_manager {
    public:
        using Pin = Abstract::Abstract_IO_pin;

        bool initialize(Pin::Pin_phase);

        Pin &get_pin(Pin_id);

        Pin *get_optional_pin(Pin_id);

        void print_diagnostics();
    };
}
