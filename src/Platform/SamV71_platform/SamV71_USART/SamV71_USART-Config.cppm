//
// Created by aeols on 2026-08-24.
//

module;

#include "FreeRTOSConfig.h"
#include "samv71q21b.h"
#include "component/usart.h"

module Platform.SamV71_USART:Config;

namespace SamV71
{
    struct SamV71_USART_config
    {
        usart_registers_t* the_base;
        uint32_t the_IRQ_priority;
        IRQn_Type the_IRQ_number;
        uint32_t the_peripheral_id;
        uint8_t the_ID_number;

        static const SamV71_USART_config* get_config(uint8_t id);
    };

    const SamV71_USART_config Config_USART1{
        .the_base = USART1_REGS,
        .the_IRQ_priority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY,
        .the_IRQ_number = USART1_IRQn,
        .the_peripheral_id = ID_USART1,
        .the_ID_number = 1,
    };

    const SamV71_USART_config* SamV71_USART_config::get_config(const uint8_t id) { return id == 1 ? &Config_USART1 : nullptr; }
}
