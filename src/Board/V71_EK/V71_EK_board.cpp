//
// Created by aeols on 12.08.2026.
//

#include "V71_EK_board.h"

#include "Abstract_clock.h"

namespace Board
{
    static void NVIC_Initialize()
    {
        /* Priority 0 to 7 and no sub-priority. 0 is the highest priority */
        NVIC_SetPriorityGrouping(0x00);
        /* Enable NVIC Controller */
        __DMB();
        __enable_irq();
        /* Enable the interrupt sources and configure the priorities as configured
         * from within the "Interrupt Manager" of MHC. */
        /* Enable Usage fault */
        SCB->SHCSR |= (SCB_SHCSR_USGFAULTENA_Msk);
        /* Trap divide by zero */
        SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;
        /* Enable Bus fault */
        SCB->SHCSR |= (SCB_SHCSR_BUSFAULTENA_Msk);
        /* Enable memory management fault */
        SCB->SHCSR |= (SCB_SHCSR_MEMFAULTENA_Msk);
    }

    void V71_EK_board::initialize()
    {
        the_clock.initialize();
        __asm volatile ("bkpt #0");
        while (true)
        {
            __NOP();
        }
        the_pin_manager.initialize();
        the_UART.initialize();
        the_console.initialize();
        NVIC_Initialize();
    }

    void V71_EK_board::print_diagnostics()
    {
        the_pin_manager.print_diagnostics();
    }
} // Board
