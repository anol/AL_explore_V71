//
// Created by aeols on 2026-11-11.
//

module;

#include "FreeRTOSConfig.h"
#include "samv71q21b.h"
#include "component/afec.h"

module Platform.SamV71_ADC:Define;

namespace SamV71
{
    struct SamV71_ADC_define
    {
        afec_registers_t* the_base;
        uint32_t the_IRQ_priority;
        IRQn_Type the_IRQ_number;
        uint32_t the_peripheral_id;
        uint8_t the_ID_number;

        static const SamV71_ADC_define* get_definition(uint8_t id);
    };

    const SamV71_ADC_define Define_AFEC0{
        .the_base = AFEC0_REGS,
        .the_IRQ_priority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY,
        .the_IRQ_number = AFEC0_IRQn,
        .the_peripheral_id = ID_AFEC0,
        .the_ID_number = 0,
    };

    const SamV71_ADC_define* SamV71_ADC_define::get_definition(const uint8_t id) { return id == 0 ? &Define_AFEC0 : nullptr; }
}
