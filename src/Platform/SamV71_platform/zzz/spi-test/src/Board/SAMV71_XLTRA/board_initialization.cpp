//
// Created by anolsen on 09.09.2019.
//

#include <cstdint>
#include <samv71q21b.h>
#include <samv71q21b_pio.h>
#include "board_definitions.h"
#include "board_initialization.h"
#include "cortex_m4_definitions.h"
#include <component/pmc.h>
#include <component/wdt.h>
#include <component/efc.h>
#include <component/matrix.h>
#include "clock/sysclk.h"

static inline void tcm_disable(void) {
    __asm volatile ("dsb"); // Data Synchronization Barrier
    __asm volatile ("isb"); // Instruction Synchronization Barrier
    SCB->ITCMCR &= ~(uint32_t)(1UL);
    SCB->DTCMCR &= ~(uint32_t) SCB_DTCMCR_EN_Msk;
    __asm volatile ("dsb"); // Data Synchronization Barrier
    __asm volatile ("isb"); // Instruction Synchronization Barrier
}

static void SCB_EnableICache (void)
{
    __asm volatile ("dsb"); // Data Synchronization Barrier
    __asm volatile ("isb"); // Instruction Synchronization Barrier
    SCB->ICIALLU = 0UL;                     /* invalidate I-Cache */
    __asm volatile ("dsb"); // Data Synchronization Barrier
    __asm volatile ("isb"); // Instruction Synchronization Barrier
    SCB->CCR |=  (uint32_t)SCB_CCR_IC_Msk;  /* enable I-Cache */
    __asm volatile ("dsb"); // Data Synchronization Barrier
    __asm volatile ("isb"); // Instruction Synchronization Barrier
}

void board_init(void) {
    sysclk_init();
    /* Disable the watchdog */
    WDT->WDT_MR = WDT_MR_WDDIS;
    /* TCM Configuration */
    EFC->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_CGPB
                     | EEFC_FCR_FARG(8));
    EFC->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_CGPB
                     | EEFC_FCR_FARG(7));
    tcm_disable();
    /* Initialize IOPORTs */
    sysclk_enable_peripheral_clock(ID_PIOA);
    sysclk_enable_peripheral_clock(ID_PIOB);
    sysclk_enable_peripheral_clock(ID_PIOC);
    sysclk_enable_peripheral_clock(ID_PIOD);
    sysclk_enable_peripheral_clock(ID_PIOE);

    MATRIX->CCFG_SYSIO |= CCFG_SYSIO_SYSIO4;

    /* Configure Push Button pins */
//    ioport_set_pin_input_mode(GPIO_PUSH_BUTTON_1, GPIO_PUSH_BUTTON_1_FLAGS,
//                              GPIO_PUSH_BUTTON_1_SENSE);

    // Enable caches
    SCB_EnableICache();
}