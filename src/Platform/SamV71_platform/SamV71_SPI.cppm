module;
#include <cstdint>

export module Platform.SamV71_SPI;
import Type.Abstract_SPI;
import Type.Abstract_IO_pin;
import Type.Transfer_request;
import Platform.FreeRTOS_queue;
import Platform.SamV71_IO_pin;

export namespace SamV71 {
    class SamV71_SPI : public Abstract::Abstract_SPI {
        enum { Queue_size = 64, SPI_bitrate = 1000000 };

        const uint8_t the_id;
        Abstract::Abstract_IO_pin &use_chip_select;
        void *optional_definition;
        bool the_busy_flag{};
        Generic::Transfer_request *optional_request{};
        FreeRTOS::FreeRTOS_queue<Abstract::Abstract_request *, Queue_size> the_queue{};
        static SamV71_SPI *optional_SPI0_driver;

    public:
        SamV71_SPI(uint8_t id, Abstract::Abstract_IO_pin &chip_select);

        void initialize() override;

        void setup_SPI_registers(uint32_t bitrate_divider) const;

        void enable_SPI() const;

        void disable_SPI() const;

        bool transfer(Abstract::Abstract_request *request) override;

        void on_interrupt();

        static SamV71_SPI *get_SPI0() { return optional_SPI0_driver; }

    private:
        bool is_transaction_complete();

        void execute_pending_transaction();

        void end_transaction() const;

        [[nodiscard]] bool on_rx_ready(uint8_t data) const;

        [[nodiscard]] bool on_tx_ready(uint8_t *data) const;

        [[nodiscard]] bool is_ready() const { return !optional_request; }
        void set_ready() { optional_request = nullptr; };
        void select_chip() const { use_chip_select.clear(); }
        void unselect_chip() const { use_chip_select.set(); }
    };
} // SamV71
