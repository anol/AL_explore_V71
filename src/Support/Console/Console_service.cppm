//
// Created by aeols on 2026-09-10.
//

export module Support.Console_service;
import :Console_receive_task;
import :Console_transmit_task;
import Support.Abstract_command_table;
import Support.Instruction_major;
import Type.Abstract_provider;
import Type.Abstract_service;
import Type.Abstract_UART;

namespace Console
{
    export class Console_service : public Abstract::Abstract_service
    {
        Console_receive_task the_receiver;
        Console_transmit_task the_transmitter{};

    public:
        Console_service(Abstract::Abstract_UART& UART, Abstract::Abstract_provider<Instruction_major>& router,
                         Instruction::Abstract_command_table& command_table) :
            the_receiver(UART, router, command_table)
        {
        }

        void initialize() override;
    };
} // Console
