module;
#include <cstdint>

export module Platform.Mockup_SPI;
import Type.Abstract_SPI;
import Type.Abstract_IO_pin;

export namespace Mockup {
    /**
     * SPI host with simulated register-file devices behind it, one per chip select.
     *
     * Frame format (as used by the IDE3380): byte 0 = (address << 1) | write_flag, followed by the
     * register value MSB first. A write stores the value; every transfer answers with the (new) value
     * of the addressed register in bytes 1.., so write-then-readback checks succeed.
     *
     * Transfers complete synchronously in the caller's context; no semaphore is involved.
     */
    class Mockup_SPI : public Abstract::Abstract_SPI {
        enum { Max_devices = 8, Register_count = 128, Max_frame_size = 16 };

        struct Device {
            const Abstract::Abstract_IO_pin *optional_chip_select{};
            uint32_t the_register[Register_count]{};
        };

        Device the_device[Max_devices]{};
        uint32_t the_transfer_count{};
        uint32_t the_write_count{};
        uint32_t the_failure_count{};

    public:
        void initialize() override {
        }

        bool transfer(Abstract::Abstract_request *request) override;

        void print_diagnostics() const;

    private:
        Device *find_device(const Abstract::Abstract_IO_pin &chip_select);
    };
} // Mockup
