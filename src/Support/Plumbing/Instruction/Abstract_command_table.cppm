//
// Created by aeols on 2026-09-23.
//

module;

export module Support.Abstract_command_table;
export import Support.Instruction_token;

export namespace Instruction {
    class Abstract_command_table {
    public:
        virtual ~Abstract_command_table() = default;

        [[nodiscard]] virtual const Instruction_token *get_commands() const = 0;
    };
} // Instruction
