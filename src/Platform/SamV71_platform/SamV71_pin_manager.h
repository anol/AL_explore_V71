#pragma once

#include "IO_pin_interface.h"
#include "Dictionary/RMU_error_codes.h"

namespace SamV71_pin_manager {
    using Pin = Interface::IO_pin_interface;

    [[nodiscard]] RMU_error_codes::Error_code initialize(Pin::Pin_phase);

    Pin &get_pin(Pin::Pin_id);

    Pin *get_optional_pin(Pin::Pin_id);

    void print_diagnostics();

    void report_all(Service_report & report);

    void report(Service_report &report, uint8_t id);
};
