module;
#include <cstdint>

export module Type.Transfer_request;

export import Type.Abstract_request;
export import Type.Abstract_semaphore;
import Type.Abstract_IO_pin;

namespace Generic {
    using namespace Abstract;

    export class SPI_transfer_request : public Abstract_request {
        Abstract_IO_pin &use_chip_select;
        Abstract_semaphore &use_semaphore;
        const uint8_t the_data_size;
        uint8_t the_send_count{};
        uint8_t the_receive_count{};
        uint8_t *optional_send_buffer{};
        uint8_t *optional_receive_buffer{};

    public:
        /**
         * @name Transfer_request constructor
         * @param chip_select The chip select pin.
         * @param data_size The transfer size in BYTES.
         * @param semaphore The semaphore instance to be given when transfer is completed.
         **/
        SPI_transfer_request(Abstract_IO_pin &chip_select, Abstract_semaphore &semaphore, const uint8_t data_size)
            : use_chip_select(chip_select), use_semaphore(semaphore), the_data_size(data_size) {
        }

        [[nodiscard]] Abstract_semaphore &get_semaphore() const override { return use_semaphore; }
        [[nodiscard]] Abstract_IO_pin &get_chip_select() const { return use_chip_select; }
        [[nodiscard]] uint8_t *get_transmit_buffer() const { return optional_send_buffer; }
        [[nodiscard]] uint8_t *get_receive_buffer() const { return optional_receive_buffer; }
        void count_sent() { the_send_count++; }
        void count_received() { the_receive_count++; }
        [[nodiscard]] bool is_more_to_send() const { return the_send_count < the_data_size; }
        [[nodiscard]] bool is_more_to_receive() const { return the_receive_count < the_data_size; }
        [[nodiscard]] uint8_t get_send_count() const { return the_send_count; }
        [[nodiscard]] uint8_t get_receive_count() const { return the_receive_count; }

        void set_buffers(uint8_t *transmit, uint8_t *receive) {
            the_send_count = 0;
            the_receive_count = 0;
            optional_send_buffer = transmit;
            optional_receive_buffer = receive;
        }
    };
} // Abstract
