//
// Created by aeols on 2026-11-19.
//

module;

#include "FreeRTOSConfig.h"
#include "samv71q21b.h"
#include "component/twihs.h"

module Platform.SamV71_I2C:Define;

namespace SamV71
{
    struct SamV71_I2C_define
    {
        twihs_registers_t* the_base;
        uint32_t the_IRQ_priority;
        IRQn_Type the_IRQ_number;
        uint32_t the_peripheral_id;
        uint8_t the_ID_number;

        static const SamV71_I2C_define* get_definition(uint8_t id);
    };

    const SamV71_I2C_define Define_I2C0{
        .the_base = TWIHS0_REGS,
        .the_IRQ_priority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY,
        .the_IRQ_number = TWIHS0_IRQn,
        .the_peripheral_id = ID_TWIHS0,
        .the_ID_number = 0,
    };

    const SamV71_I2C_define* SamV71_I2C_define::get_definition(const uint8_t id) { return id == 0 ? &Define_I2C0 : nullptr; }
}
