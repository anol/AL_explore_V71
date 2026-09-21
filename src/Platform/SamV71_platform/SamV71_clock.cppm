

module;
#include <cstdint>

export module Platform.SamV71_clock;
import Type.Abstract_clock;



export namespace SamV71
{
    class SamV71_clock : public Abstract::Abstract_clock
    {
        volatile uint32_t milliseconds_allmost_since_start{};

    public:
        SamV71_clock() = default;

        void initialize() override;

        [[nodiscard]] uint32_t get_milliseconds() const override { return milliseconds_allmost_since_start; }

        static uint32_t get_frequency();

        static void enable_peripheral_clock(uint32_t peripheral_id);

    private:
        static void initialize_main_clock();

        static void initialize_PLLA();

        static void initialize_master_clock();

        static void disable_watchdog();
    };
}
