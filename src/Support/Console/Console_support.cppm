//
// Created by aeols on 2026-09-10.
//

module;

#include <cstdint>

export module Support.Console_support;
import :Console_receive_task;
import :Console_transmit_task;
import Platform.Common_stdio;
import Support.Abstract_command_table;
import Support.Instruction_major;
import Type.Abstract_provider;
import Type.Abstract_service;
import Type.Abstract_UART;
import Support.CLI_help;

namespace Support {
    export class Console_support : public Abstract::Abstract_service {
        Platform::Common_stdio the_stdio;
        Console_receive_task the_receiver;
        Console_transmit_task the_transmitter{};
        Instruction::CLI_help &use_help;
        const uint32_t the_number_of_keys;

    public:
        Console_support(Abstract::Abstract_UART &UART, const Instruction::Abstract_command_table &command_table,
                        Instruction::CLI_help &help, const uint32_t number_of_keys) :
            the_stdio(&UART),
            the_receiver(UART, command_table),
            use_help(help),
            the_number_of_keys(number_of_keys) {}

        void initialize() override;

        void attach_router(Abstract::Abstract_provider<Instruction_major> &router) {
            the_receiver.attach_router(router);
        }

        [[nodiscard]] bool toggle_echo() const { return the_receiver.toggle_echo(); }

        [[nodiscard]] bool print_help(uint8_t command, bool is_question, bool is_sub_question, bool is_AT_mode) const;
    };
} // Support
