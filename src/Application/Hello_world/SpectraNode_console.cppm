export module Application.SpectraNode_console;
import Application.SpectraNode_command_table;
import Support.Console_support;
import Support.Instruction_major;
import Type.Abstract_provider;
import Type.Abstract_UART;
import Support.CLI_help;
import Domain.Keyword_lookup;

export namespace Application {
    class SpectraNode_console {
        // Declared first: Console_support reads the table while it is constructed.
        const SpectraNode_interface::SpectraNode_command_table the_command_table{};
        Instruction::CLI_help the_help{
                Domain_interface::Key_HELP, the_command_table.get_commands(), Domain_interface::get_keyword
        };
        Support::Console_support the_console_support;

    public:
        explicit SpectraNode_console(Abstract::Abstract_UART &UART)
            : the_console_support(UART, the_command_table, the_help, Domain_interface::number_of_keys) {}

        void attach_router(Abstract::Abstract_provider<Instruction_major> &router) {
            the_console_support.attach_router(router);
        }

        void initialize() {
            the_console_support.initialize();
        }

        Support::Console_support &get_console() { return the_console_support; }
    };
} // Application
