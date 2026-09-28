module;
#include <cstdint>

export module Type.Transfer_request;

export import Type.Abstract_request;
export import Type.Abstract_semaphore;

namespace Generic
{
    using Semaphore = Abstract::Abstract_semaphore;

    export class Transfer_request : public Abstract::Abstract_request
    {
        const uint8_t the_chip_select;
        const uint8_t the_data_width;
        const uint8_t the_transfer_size;
        uint8_t the_send_count{};
        uint8_t the_receive_count{};
        Semaphore* const optional_semaphore;
        uint8_t* optional_send_buffer{};
        uint8_t* optional_receive_buffer{};

    public:
        /**
         * @name Transfer_request constructor
         * @param chip_select The chip select id
         * @param data_width Data width in bits
         * @param semaphore A pointer to a semaphore instance.
         **/
        Transfer_request(const uint8_t chip_select, const uint8_t data_width, Semaphore* semaphore)
            : the_chip_select(chip_select), the_data_width(data_width), the_transfer_size(data_width / 8),
              optional_semaphore(semaphore)
        {
        }

        [[nodiscard]] Semaphore* get_semaphore() const override { return optional_semaphore; }
        [[nodiscard]] uint8_t get_chip_select() const { return the_chip_select; }
        [[nodiscard]] uint8_t get_data_width() const { return the_data_width; }
        [[nodiscard]] uint8_t get_transfer_size() const { return the_transfer_size; }
        [[nodiscard]] uint8_t* get_transmit_buffer() const { return optional_send_buffer; }
        [[nodiscard]] uint8_t* get_receive_buffer() const { return optional_receive_buffer; }
        void count_sent() { the_send_count++; }
        void count_received() { the_receive_count++; }
        [[nodiscard]] bool is_more_to_send() const { return the_send_count < the_transfer_size; }
        [[nodiscard]] bool is_more_to_receive() const { return the_receive_count < the_transfer_size; }
        [[nodiscard]] uint8_t get_send_count() const { return the_send_count; }
        [[nodiscard]] uint8_t get_receive_count() const { return the_receive_count; }

        void set_buffers(uint8_t* transmit, uint8_t* receive)
        {
            the_send_count = 0;
            the_receive_count = 0;
            optional_send_buffer = transmit;
            optional_receive_buffer = receive;
        }

        void set_transmit_buffer(uint8_t* transmit) { optional_send_buffer = transmit; }
        void set_receive_buffer(uint8_t* receive) { optional_receive_buffer = receive; }
    };
} // Abstract
