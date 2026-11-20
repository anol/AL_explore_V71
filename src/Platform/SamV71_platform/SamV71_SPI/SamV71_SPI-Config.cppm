//
// Created by aeols on 2026-11-04.
//

module;

#include "FreeRTOSConfig.h"
#include "samv71q21b.h"
#include "component/spi.h"

module Platform.SamV71_SPI:Config;

namespace SamV71
{
    struct SamV71_SPI_config
    {
        spi_registers_t* the_base;
        uint32_t the_IRQ_priority;
        IRQn_Type the_IRQ_number;
        uint8_t the_ID_number;

        static const SamV71_SPI_config* get_config(uint8_t id);
    };

    const SamV71_SPI_config Config_SPI0{
        .the_base = SPI0_REGS,
        .the_IRQ_priority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY,
        .the_IRQ_number = SPI0_IRQn,
        .the_ID_number = 0,
    };

    const SamV71_SPI_config* SamV71_SPI_config::get_config(const uint8_t id) { return id == 0 ? &Config_SPI0 : nullptr; }
}
