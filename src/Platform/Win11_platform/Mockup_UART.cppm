module;
#include <atomic>
#include <cstdint>

export module Platform.Mockup_UART;
import Type.Abstract_UART;
import Type.Misc_type;

export namespace Mockup {
    /**
     * Console UART on the host's stdin/stdout.
     *
     * A native (non-FreeRTOS) reader thread blocks on stdin and pushes characters into a lock-free
     * single-producer/single-consumer ring buffer; get() pops from it without ever blocking in Windows.
     * When the buffer is empty, get() sleeps a couple of RTOS ticks, like the SamV71 USART's queue
     * receive does, so a polling console task does not starve lower-priority tasks.
     */
    class Mockup_UART : public Abstract::Abstract_UART {
        enum : uint32_t { RX_buffer_size = 256, Poll_millis = 2 };

        uint8_t the_RX_buffer[RX_buffer_size]{};
        std::atomic<uint32_t> the_head{};  // written by the reader thread
        std::atomic<uint32_t> the_tail{};  // written by the consumer
        std::atomic<uint32_t> the_overflow_count{};
        bool the_echo_flag{};
        bool the_started_flag{};

    public:
        void initialize() override;

        bool for_each_input(Optional_user user, Optional_func func) override;

        bool is_ready() override { return true; }

        int print(const char *data, int len) override;

        bool toggle_echo() override { return the_echo_flag = !the_echo_flag; }

        [[nodiscard]] Status_code put(uint8_t data) override;

        [[nodiscard]] Status_code get(uint8_t *data) override;

        /// Inject input as if typed (e.g. from a test). Safe from one producer at a time.
        void inject(const char *text);

        /// Called by the reader thread.
        void on_input(uint8_t data);

    private:
        bool pop(uint8_t &data);

        static void wait_a_little();
    };
} // Mockup
