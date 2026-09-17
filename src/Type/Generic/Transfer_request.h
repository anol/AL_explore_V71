#pragma once

#include <cstdint>

#include "Abstract_semaphore.h"

namespace Generic {
    using Semaphore = Abstract::Abstract_semaphore;

    class Transfer_request {
        const uint8_t    the_chip_select;
        const uint8_t    the_data_size;
        Semaphore *const optional_semaphore;
        uint8_t *        optional_transmit_buffer{};
        uint8_t *        optional_receive_buffer{};

    public:
        Transfer_request(const uint8_t chip_select, const uint8_t data_size, Semaphore *semaphore)
            : the_chip_select(chip_select), the_data_size(data_size), optional_semaphore(semaphore) {
        }

        virtual ~Transfer_request() = default;

        [[nodiscard]] uint8_t    get_chip_select() const { return the_chip_select; }
        [[nodiscard]] uint8_t    get_data_size() const { return the_data_size; }
        [[nodiscard]] Semaphore *get_semaphore() const { return optional_semaphore; }
        [[nodiscard]] uint8_t *  get_transmit_buffer() const { return optional_transmit_buffer; }
        [[nodiscard]] uint8_t *  get_receive_buffer() const { return optional_receive_buffer; }
        void                     set_transmit_buffer(uint8_t *transmit) { optional_transmit_buffer = transmit; }
        void                     set_receive_buffer(uint8_t *receive) { optional_receive_buffer = receive; }
    };
} // Abstract
