//
// Created by aeols on 2026-09-10.
//

module;

#include <cstdint>

module Support.Console_support;

namespace Support {
    void Console_support::initialize() {
        the_stdio.initialize();
        the_receiver.initialize();
        the_transmitter.initialize();
    }

    bool Console_support::print_help(
            const uint8_t command, const bool is_question, const bool is_sub_question, const bool is_AT_mode) const {
        use_help.set_mode(is_AT_mode);
        if (use_help.is_help_command(command) || is_question) {
            use_help.print(is_sub_question, the_number_of_keys);
        } else if (is_sub_question) {
            use_help.print(static_cast<std::uint32_t>(command));
        } else {
            use_help.print(false, the_number_of_keys);
        }
        return true;
    }
} // Support
