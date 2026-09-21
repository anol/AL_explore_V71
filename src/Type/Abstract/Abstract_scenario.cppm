export module Type.Abstract_scenario;

export import Type.Status_code;

namespace Abstract {
    /// The Request type is opaque to this interface: it is whatever the caller uses to start (and later
    /// acknowledge) a test. Keeping it a template parameter avoids any dependency from Type on the caller.
    export template<class Request>
    class Abstract_scenario {
        bool is_trace_flag{};

    public:
        virtual ~Abstract_scenario() = default;

        [[nodiscard]] virtual bool is_active() const = 0;

        [[nodiscard]] virtual Status_code start_test(Request *request) = 0;

        virtual void background_process() = 0;

        virtual void print_diag() const = 0;

        void set_trace(bool trace) { is_trace_flag = trace; }

        [[nodiscard]] bool is_trace() const { return is_trace_flag; }
    };
}
