module;
#include <cstdint>

export module Platform.SamV71_USART;
import Type.Abstract_UART;
import Type.Misc_type;
import Platform.FreeRTOS_queue;

export namespace SamV71 {
    class SamV71_USART : public Abstract::Abstract_UART {
        enum { RX_queue_size = 128, TX_queue_size = 4096 };

        const uint8_t the_id;
        void *optional_definition;
        FreeRTOS::FreeRTOS_queue<uint8_t, RX_queue_size> the_RX_queue{};
        FreeRTOS::FreeRTOS_queue<uint8_t, TX_queue_size> the_TX_queue{};

    public:
        void ISR();

    private:
        void ISR_RX_ready(uint32_t &context_switch);

        void ISR_TX_ready(uint32_t &context_switch);

    public:
        explicit SamV71_USART(uint8_t id);

        void initialize() override;

        bool is_ready() override { return true; }

        Status_code put(const uint8_t data) override {
            return Status_code(the_TX_queue.send(data));
        }

        Status_code get(uint8_t *data) override {
            return Status_code(the_RX_queue.receive(data));
        }

        bool for_each_input(Optional_user user, Optional_func func) override;

        int print(const char *data, int len) override;

    private:
        void init_USART();

        void init_interrupt();

        void enable_receiver_interrupt();

        void disable_receiver_interrupt();

        void enable_transmitter_interrupt();

        void disable_transmitter_interrupt();
    };
} // SamV71
