module;
#include <cstdint>

export module Platform.Mockup_I2C;
import Type.Abstract_I2C;

export namespace Mockup {
    /**
     * I2C host with simulated register-pointer devices behind it, one per 7-bit device address
     * (allocated on first access, so every address ACKs until all slots are taken).
     *
     * Device model (as used by e.g. the STTS22H): bytes are carried MSB first. A write sets the
     * register pointer from its first byte and stores any following bytes from there on; a read
     * returns bytes starting at the register pointer. The pointer auto-increments per data byte.
     *
     * Transfers complete synchronously in the caller's context; no semaphore is involved.
     */
    class Mockup_I2C : public Abstract::Abstract_I2C {
        enum { Max_devices = 8, Register_count = 256, Max_data_width = 4 };

        struct Device {
            bool is_used{};
            uint8_t the_address{};
            uint8_t the_pointer{};
            uint8_t the_register[Register_count]{};
        };

        Device the_device[Max_devices]{
            {true, 72, 0, {20, 1, 0}},
            {true, 73, 0, {20, 2, 0}},
            {true, 74, 0, {20, 3, 0}},
            {true, 75, 0, {20, 4, 0}},
            {true, 76, 0, {20, 5, 0}},
            {true, 77, 0, {20, 6, 0}},
            {false, 0, 0, {20, 7, 0}},
            {false, 0, 0, {20, 8, 0}}
        };
        uint32_t the_read_count{};
        uint32_t the_write_count{};
        uint32_t the_failure_count{};

    public:
        void initialize() override {
        }

        bool read(uint8_t address, uint8_t data_width, uint32_t &data) override;

        bool write(uint8_t address, uint8_t data_width, uint32_t data) override;

        void print_diagnostics() const;

    private:
        Device *find_device(uint8_t address);
    };
} // Mockup
