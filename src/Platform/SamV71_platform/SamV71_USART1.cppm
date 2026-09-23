module;
#include <cstdint>
#include "FreeRTOS.h"
#include "queue.h"

export module Platform.SamV71_USART1;
import Type.Abstract_UART;
import Type.Misc_type;
import Platform.FreeRTOS_queue;


export namespace SamV71
{
    class SamV71_USART1 : public Abstract::Abstract_UART
    {
    public:
        static SamV71_USART1* optional_one_and_only_UART;
        static uint32_t the_USART1_IRQ_count;
        static uint32_t the_USART1_IRQ_status;
        static uint32_t the_USART1_TX_count;
        static uint32_t the_USART1_TX_overflow_count;
        static uint32_t the_USART1_RX_count;
        static uint32_t the_USART1_RX_overflow_count;

    private:
        enum { RX_queue_size = 128, TX_queue_size = 4096 };

        FreeRTOS::FreeRTOS_queue<uint8_t, RX_queue_size> the_RX_queue{};
        FreeRTOS::FreeRTOS_queue<uint8_t, TX_queue_size> the_TX_queue{};

    public:
        SamV71_USART1() = default;

        void initialize() override;

        bool is_ready() override { return true; }

        Status_code put(const uint8_t data) override { return Status_code(the_TX_queue.send(data)); }

        Status_code get(uint8_t* data) override { return Status_code(the_RX_queue.receive(data)); }

        bool for_each_input(Optional_user user, Optional_func func) override;

        int print(const char* data, int len) override;

        void on_receiver_interrupt(BaseType_t* pxHigherPriorityTaskWoken);

        void on_transmitter_interrupt(BaseType_t* pxHigherPriorityTaskWoken);

    private:
        void init_USART1();

        static void init_interrupt();

        static void enable_receiver_interrupt();

        static void disable_receiver_interrupt();

        static void enable_transmitter_interrupt();

        static void disable_transmitter_interrupt();
    };
} // SamV71
