#pragma once

#include <cstdint>

import Type.Abstract_request;
import Type.Abstract_semaphore;

namespace Generic
{
    using Semaphore = Abstract::Abstract_semaphore;

    class Transfer_request : public Abstract::Abstract_request
    {
        const uint8_t the_chip_select;
        const uint8_t the_data_width;
        Semaphore* const optional_semaphore;
        uint8_t* optional_transmit_buffer{};
        uint8_t* optional_receive_buffer{};

    public:
        /**
         * @name Transfer_request constructor
         * @param chip_select The chip select id
         * @param data_width Data width in bits
         * @param semaphore A pointer to a semaphore instance.
         **/
        Transfer_request(const uint8_t chip_select, const uint8_t data_width, Semaphore* semaphore)
            : the_chip_select(chip_select), the_data_width(data_width), optional_semaphore(semaphore)
        {
        }

        [[nodiscard]] Semaphore* get_semaphore() const override { return optional_semaphore; }

        [[nodiscard]] uint8_t get_chip_select() const { return the_chip_select; }
        [[nodiscard]] uint8_t get_data_width() const { return the_data_width; }
        [[nodiscard]] uint8_t* get_transmit_buffer() const { return optional_transmit_buffer; }
        [[nodiscard]] uint8_t* get_receive_buffer() const { return optional_receive_buffer; }

        void set_buffers(uint8_t* transmit, uint8_t* receive)
        {
            optional_transmit_buffer = transmit;
            optional_receive_buffer = receive;
        }

        void set_transmit_buffer(uint8_t* transmit) { optional_transmit_buffer = transmit; }
        void set_receive_buffer(uint8_t* receive) { optional_receive_buffer = receive; }
    };
} // Abstract
