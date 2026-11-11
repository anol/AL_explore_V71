module;
#include <cstdint>
#include <cstdio>

export module Platform.Mockup_IO_pin;
import Type.Abstract_IO_pin;

export namespace Mockup {
    /// A GPIO pin that only remembers its state. Optionally traces changes to stdout.
    class Mockup_IO_pin : public Abstract::Abstract_IO_pin {
        const char *the_name;
        uint8_t the_id;
        bool the_state;
        bool the_trace_flag{};
        bool the_used_flag{};
        uint32_t the_change_count{};

    public:
        Mockup_IO_pin() : Mockup_IO_pin(0) {
        }

        explicit Mockup_IO_pin(const uint8_t id, const char *name = "nu", const bool default_state = false)
            : the_name(name), the_id(id), the_state(default_state) {
        }

        void configure(const uint8_t id, const char *name, const bool default_state) {
            the_id = id;
            the_name = name;
            the_state = default_state;
        }

        [[nodiscard]] Status_code initialize(const uint8_t id, Pin_phase) override {
            the_id = id;
            the_used_flag = true;
            return Status_code::Success();
        }

        [[nodiscard]] uint8_t get_id() const override { return the_id; }

        [[nodiscard]] bool get() const override { return the_state; }

        void set() override { change(true); }

        void clear() override { change(false); }

        void toggle() override { change(!the_state); }

        void pulse() override {
            toggle();
            toggle();
        }

        void pulse(const uint32_t count) override {
            for (uint32_t index = 0; index < count; index++) {
                pulse();
            }
        }

        [[nodiscard]] bool is_used() const override { return the_used_flag; }

        void set_trace(const bool trace) { the_trace_flag = trace; }

        void print_diagnostics() const {
            printf("  pin %2d %-12s state=%d changes=%u\r\n", the_id, the_name, the_state ? 1 : 0,
                   static_cast<unsigned>(the_change_count));
        }

    private:
        void change(const bool state) {
            if (state != the_state) {
                the_state = state;
                the_change_count++;
                if (the_trace_flag) {
                    printf("[pin %s=%d]\r\n", the_name, state ? 1 : 0);
                }
            }
        }
    };
} // Mockup
