module;

#include "FreeRTOS.h"

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

    public:
        void ISR();

    private:
        void ISR_pending_transaction(bool &context_switch);

        bool ISR_check_progress(bool &context_switch);

        void ISR_transfer_complete(bool &context_switch) const;

        [[nodiscard]] bool ISR_RX_ready(uint8_t data, bool &context_switch) const;

        [[nodiscard]] bool ISR_TX_ready(uint8_t *data, bool &context_switch) const;

    public:
        SamV71_SPI(uint8_t id, Abstract::Abstract_IO_pin &chip_select);

        void initialize() override;

        bool transfer(Abstract::Abstract_request *request) override;

    private:
        void pending_transaction();

        void setup_SPI_registers(uint32_t bitrate_divider) const;

        void enable_SPI() const;

        void disable_SPI() const;

        [[nodiscard]] bool is_ready() const {
            return !optional_request;
        }

        void set_ready() {
            optional_request = nullptr;
        };

        void select_chip() const {
            use_chip_select.clear();
        }

        void unselect_chip() const {
            use_chip_select.set();
        }
    };
} // SamV71
