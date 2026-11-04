//
// Created by aeols on 2026-11-04.
//

module;

#include "FreeRTOSConfig.h"
#include "samv71q21b.h"
#include "component/spi.h"

module Platform.SamV71_SPI:Define;

namespace SamV71
{
    struct SamV71_SPI_define
    {
        spi_registers_t* the_base;
        IRQn_Type the_IRQ_number;
        uint32_t the_IRQ_priority;

        static const SamV71_SPI_define* get_definition(uint8_t id);
    };

    const SamV71_SPI_define Define_SPI0{
        .the_base         = SPI0_REGS,
        .the_IRQ_number   = SPI0_IRQn,
        .the_IRQ_priority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY,
    };

    const SamV71_SPI_define* SamV71_SPI_define::get_definition(const uint8_t id) { return id == 0 ? &Define_SPI0 : nullptr; }

}

