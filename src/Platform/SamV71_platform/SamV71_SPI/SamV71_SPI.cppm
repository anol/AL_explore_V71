//
// Created by aeols on 2026-11-04.
//

module;

#include "samv71q21b.h"
#include "component/spi.h"

export module Platform.SamV71_SPI;
import :Define;
import :Driver;
import :Handler;

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
            SPI_CR_clock_phase     = SPI_CSR_NCPHA(1),
            SPI_MR_host_mode       = SPI_MR_MSTR(1),
            SPI_MR_fault_detection = SPI_MR_MODFDIS(1),
            SPI_bitrate            = 2'000'000,
        };

        const uint8_t the_id;
        Abstract::Abstract_IO_pin &use_chip_select;
        const SamV71_SPI_define *optional_define;
        FreeRTOS::FreeRTOS_queue<Abstract::Abstract_request *, Queue_size> the_queue{};

    public:
        SamV71_SPI(uint8_t id, Abstract::Abstract_IO_pin &chip_select);

        void initialize() override;

        bool transfer(Abstract::Abstract_request *request) override;

        void print_diagnostics() const;

        [[nodiscard]] uint8_t get_id() const { return the_id; }

        [[nodiscard]] const SamV71_SPI_define *get_definition() const { return optional_define; }

        void enable_SPI() const;

        void disable_SPI() const;

        Abstract::Abstract_request *ISR_receive(uint32_t &context_switch) {
            Abstract::Abstract_request *request;
            if (the_queue.ISR_receive(&request, context_switch)) {
                return request;
            }
            return {};
        }

    private:
        void setup_SPI_registers() const;
    };
} // SamV71
