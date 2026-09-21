

module;
#include <cstdint>

export module Platform.SamV71_SPI;
import Type.Abstract_SPI;
import Type.Transfer_request;
import Platform.FreeRTOS_queue;



export namespace SamV71 {
    class SamV71_SPI : public Abstract::Abstract_SPI {
        enum { Queue_size = 64, SPI_bitrate = 1000000 };

        FreeRTOS::FreeRTOS_queue<void *, Queue_size> the_queue{};

    public:
        void initialize() override;

        void setup_SPI_registers(uint32_t bitrate_divider) const;

        void enable_SPI() const;

        void disable_SPI() const;

        bool transfer(Abstract::Abstract_request *request) override;
    };
} // SamV71
