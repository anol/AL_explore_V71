#pragma once

import Type.Status_code;

class Instruction_major;

namespace Abstract {
    class Abstract_scenario {
        bool is_trace_flag{};

    public:
        virtual ~Abstract_scenario() = default;

        [[nodiscard]] virtual bool is_active() const = 0;

        [[nodiscard]] virtual Status_code start_test(Instruction_major *instruction) = 0;

        virtual void background_process() = 0;

        virtual void print_diag() const = 0;

        void set_trace(bool trace) { is_trace_flag = trace; }

        [[nodiscard]] bool is_trace() const { return is_trace_flag; }
    };
}
