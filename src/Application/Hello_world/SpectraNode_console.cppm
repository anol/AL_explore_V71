export module Application.SpectraNode_console;
import Domain.SpectraNode_command_table;
import Support.Console_support;
import Support.Instruction_major;
import Type.Abstract_provider;
import Type.Abstract_UART;

export namespace Application
{
    class SpectraNode_console
    {
        // Declared first: Console_support reads the table while it is constructed.
        const SpectraNode_interface::SpectraNode_command_table the_command_table{};
        Support::Console_support the_console_support;

    public:
        SpectraNode_console(Abstract::Abstract_UART& UART, Abstract::Abstract_provider<Instruction_major>& router)
            : the_console_support(UART, router, the_command_table)
        {
        }

        void initialize()
        {
            the_console_support.initialize();
        }
    };
} // Application
