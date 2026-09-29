module;

#include "samv71q21b.h"
#include "component/spi.h"

export module Platform.SamV71_SPI;
import Type.Abstract_SPI;
import Type.Abstract_IO_pin;
import Type.Transfer_request;
import Platform.FreeRTOS_queue;
import Platform.SamV71_IO_pin;

export namespace SamV71 {
    class SamV71_SPI : public Abstract::Abstract_SPI {
        enum {
            Queue_size             = 64,
            SPI_CR_clock_polarity  = SPI_CSR_CPOL(0),
            SPI_CR_clock_phase     = SPI_CSR_NCPHA(0),
            SPI_MR_host_mode       = SPI_MR_MSTR(1),
            SPI_MR_fault_detection = SPI_MR_MODFDIS(1),
            SPI_bitrate            = 1000000,
        };

        const uint8_t the_id;
        Abstract::Abstract_IO_pin &use_chip_select;
        void *optional_definition;
        bool the_busy_flag{};
        Generic::Transfer_request *optional_request{};
        FreeRTOS::FreeRTOS_queue<Abstract::Abstract_request *, Queue_size> the_queue{};

    public:
        void ISR();

    private:
        void ISR_pending_transaction(uint32_t &context_switch);

        void ISR_check_progress(uint32_t &context_switch);

        void ISR_transfer_complete(uint32_t &context_switch);

        void ISR_RX_ready() const;

        void ISR_TX_ready() const;

    public:
        SamV71_SPI(uint8_t id, Abstract::Abstract_IO_pin &chip_select);

        void initialize() override;

        bool transfer(Abstract::Abstract_request *request) override;

        void print_diagnostics() const;

    private:
        void pending_transaction();

        void setup_SPI_registers() const;

        void enable_SPI() const;

        void disable_SPI() const;

        [[nodiscard]] bool is_ready() const {
            return !optional_request;
        }

        void set_ready() {
            optional_request = nullptr;
        }

        void select_chip() const {
            use_chip_select.clear();
        }

        void unselect_chip() const {
            use_chip_select.set();
        }
    };
} // SamV71
