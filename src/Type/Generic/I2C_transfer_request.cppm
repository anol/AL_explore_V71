module;
#include <cstdint>

export module Type.I2C_transfer_request;

export import Type.Abstract_request;
export import Type.Abstract_semaphore;

namespace Generic {
    using namespace Abstract;

    /**
     * One I2C host transaction (a single START ... STOP frame) of 1..4 data bytes.
     *
     * The value is carried MSB first on the bus: for a width of N bytes, byte 0 on the bus is
     * bits [8N-1 .. 8N-8] of the value, and the last byte on the bus is bits [7 .. 0].
     */
    export class I2C_transfer_request : public Abstract_request {
    public:
        enum { Max_data_size = 4 };

    private:
        Abstract_semaphore &use_semaphore;
        const uint8_t the_device_address;
        const bool the_read_flag;
        const uint8_t the_data_size;
        uint8_t the_count{};
        bool the_success_flag{};
        uint8_t the_buffer[Max_data_size]{};

    public:
        /**
         * @param semaphore The semaphore instance to be given when the transaction is completed.
         * @param device_address The 7-bit device address.
         * @param is_read True for a host read, false for a host write.
         * @param data_size The transfer size in BYTES (1..4); larger values are clamped to 4.
         * @param value The value to write (ignored for a read).
         **/
        I2C_transfer_request(Abstract_semaphore &semaphore, const uint8_t device_address, const bool is_read,
                             const uint8_t data_size, const uint32_t value = 0)
            : use_semaphore(semaphore), the_device_address(device_address & 0x7F), the_read_flag(is_read),
              the_data_size(data_size < Max_data_size ? data_size : static_cast<uint8_t>(Max_data_size)) {
            if (!is_read) {
                for (uint8_t index = 0; index < the_data_size; index++) {
                    the_buffer[index] = static_cast<uint8_t>(value >> 8 * (the_data_size - 1 - index));
                }
            }
        }

        [[nodiscard]] Abstract_semaphore &get_semaphore() const override { return use_semaphore; }
        [[nodiscard]] uint8_t get_device_address() const { return the_device_address; }
        [[nodiscard]] bool is_read() const { return the_read_flag; }
        [[nodiscard]] uint8_t get_data_size() const { return the_data_size; }
        [[nodiscard]] uint8_t get_count() const { return the_count; }
        [[nodiscard]] bool is_more_to_transfer() const { return the_count < the_data_size; }
        [[nodiscard]] uint8_t get_remaining() const { return the_data_size - the_count; }
        [[nodiscard]] bool is_success() const { return the_success_flag; }

        /** Next byte to send (host write). */
        [[nodiscard]] uint8_t get_next_byte() const { return the_count < the_data_size ? the_buffer[the_count] : 0xFF; }

        /** Store a received byte (host read). */
        void put_next_byte(const uint8_t data) {
            if (the_count < the_data_size) {
                the_buffer[the_count] = data;
            }
        }

        void count_transferred() { the_count++; }

        void set_success(const bool success) { the_success_flag = success; }

        /** The received value, assembled MSB first. */
        [[nodiscard]] uint32_t get_value() const {
            uint32_t value{};
            for (uint8_t index = 0; index < the_data_size; index++) {
                value = value << 8 | the_buffer[index];
            }
            return value;
        }
    };
} // Generic
