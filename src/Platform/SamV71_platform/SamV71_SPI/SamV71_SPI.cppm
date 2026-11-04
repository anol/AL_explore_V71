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
import Platform.SamV71_IO_pin;
import Platform.SamV71_clock;

export namespace SamV71
{
    class SamV71_SPI : public Abstract::Abstract_SPI
    {
        const uint8_t the_id;
        Abstract::Abstract_IO_pin& use_chip_select;
        SamV71_SPI_driver the_driver;
        SamV71_SPI_handler the_handler;

    public:
        SamV71_SPI(uint8_t id, Abstract::Abstract_IO_pin& chip_select)
            : the_id(id), use_chip_select(chip_select),
              the_driver(SamV71_SPI_define::get_definition(id)),
              the_handler(SamV71_SPI_define::get_definition(id), the_driver)
        {
        }

        void initialize() override;

        bool transfer(Abstract::Abstract_request* request) override { return the_driver.transfer(request); }

        void print_diagnostics() const { the_driver.print_diagnostics(); }

        [[nodiscard]] uint8_t get_id() const { return the_id; }
    };

    void SamV71_SPI::initialize()
    {
        SamV71_clock::enable_peripheral_clock(SPI0_INSTANCE_ID);
        the_driver.disable_SPI();
        the_driver.setup_SPI_registers();
        the_driver.enable_SPI();
    }
} // SamV71
