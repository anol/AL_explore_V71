#pragma once

#include <cstdint>

namespace Abstract {
    class SPI_request {
        const uint8_t the_chip_select;
        const uint8_t the_data_size;
        uint8_t *     optional_transmit_buffer{};
        uint8_t *     optional_receive_buffer{};

    public:
        SPI_request(uint8_t chip_select, uint8_t data_size)
            : the_chip_select(chip_select), the_data_size(data_size) {
        }

        virtual ~SPI_request() = default;

    protected:
        [[nodiscard]] uint8_t  get_chip_select() const { return the_chip_select; }
        [[nodiscard]] uint8_t  get_data_size() const { return the_data_size; }
        [[nodiscard]] uint8_t *get_transmit_buffer() const { return optional_transmit_buffer; }
        [[nodiscard]] uint8_t *get_receive_buffer() const { return optional_receive_buffer; }
        void                   set_transmit_buffer(uint8_t *transmit) { optional_transmit_buffer = transmit; }
        void                   set_receive_buffer(uint8_t *receive) { optional_receive_buffer = receive; }
    };

    class Abstract_SPI {
    public:
        virtual ~Abstract_SPI() = default;

        virtual void initialize() = 0;

        virtual bool transfer(SPI_request *request) = 0;
    };
} // Abstract
