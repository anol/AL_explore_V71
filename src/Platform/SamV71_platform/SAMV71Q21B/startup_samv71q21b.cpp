/*
 * GCC startup file for ATSAMV71Q21B
 *
 * Copyright (c) 2026 Microchip Technology Inc. and its subsidiaries.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "samv71q21b.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/* Optional application-provided functions */
extern void __attribute__((weak,long_call)) _on_reset();

extern void __attribute__((weak,long_call)) _on_bootstrap();

extern void __attribute__((weak,long_call)) _on_exit();

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main();

/** \endcond */

extern "C" void __libc_init_array();

/* Reset handler */
void Reset_Handler();

/* Default empty handler */
extern "C" void Dummy_Handler();

/*
void NonMaskableInt_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MemoryManagement_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void BusFault_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UsageFault_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SVCall_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DebugMonitor_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
extern "C" void SysTick_Handler() __attribute__ ((weak, alias("Dummy_SysTick_Handler")));
*/

/* Peripherals handlers */
extern "C" void SUPC_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void RSTC_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void RTC_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void RTT_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void WDT_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void PMC_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void EFC_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void UART0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void UART1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void PIOA_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void PIOB_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void PIOC_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void USART0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

// extern "C" void USART1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void USART2_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void PIOD_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void PIOE_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void HSMCI_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TWIHS0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TWIHS1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

// extern "C" void SPI0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void SSC_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TC0_CH0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TC0_CH1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TC0_CH2_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TC1_CH0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TC1_CH1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TC1_CH2_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void AFEC0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void DACC_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void PWM0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void ICM_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void ACC_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void USBHS_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void MCAN0_INT0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void MCAN0_INT1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void MCAN1_INT0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void MCAN1_INT1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void GMAC_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void AFEC1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TWIHS2_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void SPI1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void QSPI_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void UART2_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void UART3_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void UART4_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TC2_CH0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TC2_CH1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TC2_CH2_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TC3_CH0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TC3_CH1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TC3_CH2_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void MLB_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void AES_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void TRNG_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void XDMAC_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void ISI_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void PWM1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void FPU_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void RSWDT_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void CCW_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void CCF_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void GMAC_Q1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void GMAC_Q2_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void IXC_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void I2SC0_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void I2SC1_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void GMAC_Q3_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void GMAC_Q4_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

extern "C" void GMAC_Q5_Handler() __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {

    /* Configure Initial Stack Pointer, using linker-generated symbols */
    .pvStack = (void *) (&_estack),


    .pfnReset_Handler            = (void *) Reset_Handler,
    .pfnNonMaskableInt_Handler   = (void *) NonMaskableInt_Handler,
    .pfnHardFault_Handler        = (void *) HardFault_Handler,
    .pfnMemoryManagement_Handler = (void *) MemoryManagement_Handler,
    .pfnBusFault_Handler         = (void *) BusFault_Handler,
    .pfnUsageFault_Handler       = (void *) UsageFault_Handler,
    .pvReservedC9                = static_cast<void *>(nullptr), /* Reserved */
    .pvReservedC8                = static_cast<void *>(nullptr), /* Reserved */
    .pvReservedC7                = static_cast<void *>(nullptr), /* Reserved */
    .pvReservedC6                = static_cast<void *>(nullptr), /* Reserved */
    .pfnSVCall_Handler           = (void *) SVCall_Handler,
    .pfnDebugMonitor_Handler     = (void *) DebugMonitor_Handler,
    .pvReservedC3                = static_cast<void *>(nullptr), /* Reserved */
    .pfnPendSV_Handler           = (void *) PendSV_Handler,
    .pfnSysTick_Handler          = (void *) SysTick_Handler,

    /* Configurable interrupts */
    .pfnSUPC_Handler       = (void *) SUPC_Handler,        /* 0  Supply Controller */
    .pfnRSTC_Handler       = (void *) RSTC_Handler,        /* 1  Reset Controller */
    .pfnRTC_Handler        = (void *) RTC_Handler,         /* 2  Real-time Clock */
    .pfnRTT_Handler        = (void *) RTT_Handler,         /* 3  Real-time Timer */
    .pfnWDT_Handler        = (void *) WDT_Handler,         /* 4  Watchdog Timer */
    .pfnPMC_Handler        = (void *) PMC_Handler,         /* 5  Power Management Controller */
    .pfnEFC_Handler        = (void *) EFC_Handler,         /* 6  Embedded Flash Controller */
    .pfnUART0_Handler      = (void *) UART0_Handler,       /* 7  Universal Asynchronous Receiver Transmitter */
    .pfnUART1_Handler      = (void *) UART1_Handler,       /* 8  Universal Asynchronous Receiver Transmitter */
    .pvReserved9           = static_cast<void *>(nullptr), /* 9  Reserved */
    .pfnPIOA_Handler       = (void *) PIOA_Handler,        /* 10 Parallel Input/Output Controller */
    .pfnPIOB_Handler       = (void *) PIOB_Handler,        /* 11 Parallel Input/Output Controller */
    .pfnPIOC_Handler       = (void *) PIOC_Handler,        /* 12 Parallel Input/Output Controller */
    .pfnUSART0_Handler     = (void *) USART0_Handler,      /* 13 Universal Synchronous Asynchronous Receiver Transmitter */
    .pfnUSART1_Handler     = (void *) ISR_USART1,      /* 14 Universal Synchronous Asynchronous Receiver Transmitter */
    .pfnUSART2_Handler     = (void *) USART2_Handler,      /* 15 Universal Synchronous Asynchronous Receiver Transmitter */
    .pfnPIOD_Handler       = (void *) PIOD_Handler,        /* 16 Parallel Input/Output Controller */
    .pfnPIOE_Handler       = (void *) PIOE_Handler,        /* 17 Parallel Input/Output Controller */
    .pfnHSMCI_Handler      = (void *) HSMCI_Handler,       /* 18 High Speed MultiMedia Card Interface */
    .pfnTWIHS0_Handler     = (void *) TWIHS0_Handler,      /* 19 Two-wire Interface High Speed */
    .pfnTWIHS1_Handler     = (void *) TWIHS1_Handler,      /* 20 Two-wire Interface High Speed */
    .pfnSPI0_Handler       = (void *) ISR_SPI0,        /* 21 Serial Peripheral Interface */
    .pfnSSC_Handler        = (void *) SSC_Handler,         /* 22 Synchronous Serial Controller */
    .pfnTC0_CH0_Handler    = (void *) TC0_CH0_Handler,     /* 23 Timer/Counter 0 Channel 0 */
    .pfnTC0_CH1_Handler    = (void *) TC0_CH1_Handler,     /* 24 Timer/Counter 0 Channel 1 */
    .pfnTC0_CH2_Handler    = (void *) TC0_CH2_Handler,     /* 25 Timer/Counter 0 Channel 2 */
    .pfnTC1_CH0_Handler    = (void *) TC1_CH0_Handler,     /* 26 Timer/Counter 1 Channel 0 */
    .pfnTC1_CH1_Handler    = (void *) TC1_CH1_Handler,     /* 27 Timer/Counter 1 Channel 1 */
    .pfnTC1_CH2_Handler    = (void *) TC1_CH2_Handler,     /* 28 Timer/Counter 1 Channel 2 */
    .pfnAFEC0_Handler      = (void *) AFEC0_Handler,       /* 29 Analog Front-End Controller */
    .pfnDACC_Handler       = (void *) DACC_Handler,        /* 30 Digital-to-Analog Converter Controller */
    .pfnPWM0_Handler       = (void *) PWM0_Handler,        /* 31 Pulse Width Modulation Controller */
    .pfnICM_Handler        = (void *) ICM_Handler,         /* 32 Integrity Check Monitor */
    .pfnACC_Handler        = (void *) ACC_Handler,         /* 33 Analog Comparator Controller */
    .pfnUSBHS_Handler      = (void *) USBHS_Handler,       /* 34 USB High-Speed Interface */
    .pfnMCAN0_INT0_Handler = (void *) MCAN0_INT0_Handler,  /* 35 Controller Area Network */
    .pfnMCAN0_INT1_Handler = (void *) MCAN0_INT1_Handler,  /* 36 Controller Area Network */
    .pfnMCAN1_INT0_Handler = (void *) MCAN1_INT0_Handler,  /* 37 Controller Area Network */
    .pfnMCAN1_INT1_Handler = (void *) MCAN1_INT1_Handler,  /* 38 Controller Area Network */
    .pfnGMAC_Handler       = (void *) GMAC_Handler,        /* 39 Gigabit Ethernet MAC */
    .pfnAFEC1_Handler      = (void *) AFEC1_Handler,       /* 40 Analog Front-End Controller */
    .pfnTWIHS2_Handler     = (void *) TWIHS2_Handler,      /* 41 Two-wire Interface High Speed */
    .pfnSPI1_Handler       = (void *) SPI1_Handler,        /* 42 Serial Peripheral Interface */
    .pfnQSPI_Handler       = (void *) QSPI_Handler,        /* 43 Quad Serial Peripheral Interface */
    .pfnUART2_Handler      = (void *) UART2_Handler,       /* 44 Universal Asynchronous Receiver Transmitter */
    .pfnUART3_Handler      = (void *) UART3_Handler,       /* 45 Universal Asynchronous Receiver Transmitter */
    .pfnUART4_Handler      = (void *) UART4_Handler,       /* 46 Universal Asynchronous Receiver Transmitter */
    .pfnTC2_CH0_Handler    = (void *) TC2_CH0_Handler,     /* 47 Timer/Counter 2 Channel 0 */
    .pfnTC2_CH1_Handler    = (void *) TC2_CH1_Handler,     /* 48 Timer/Counter 2 Channel 1 */
    .pfnTC2_CH2_Handler    = (void *) TC2_CH2_Handler,     /* 49 Timer/Counter 2 Channel 2 */
    .pfnTC3_CH0_Handler    = (void *) TC3_CH0_Handler,     /* 50 Timer/Counter 3 Channel 0 */
    .pfnTC3_CH1_Handler    = (void *) TC3_CH1_Handler,     /* 51 Timer/Counter 3 Channel 1 */
    .pfnTC3_CH2_Handler    = (void *) TC3_CH2_Handler,     /* 52 Timer/Counter 3 Channel 2 */
    .pfnMLB_Handler        = (void *) MLB_Handler,         /* 53 MediaLB */
    .pvReserved54          = static_cast<void *>(nullptr), /* 54 Reserved */
    .pvReserved55          = static_cast<void *>(nullptr), /* 55 Reserved */
    .pfnAES_Handler        = (void *) AES_Handler,         /* 56 Advanced Encryption Standard */
    .pfnTRNG_Handler       = (void *) TRNG_Handler,        /* 57 True Random Number Generator */
    .pfnXDMAC_Handler      = (void *) XDMAC_Handler,       /* 58 Extensible DMA Controller */
    .pfnISI_Handler        = (void *) ISI_Handler,         /* 59 Image Sensor Interface */
    .pfnPWM1_Handler       = (void *) PWM1_Handler,        /* 60 Pulse Width Modulation Controller */
    .pfnFPU_Handler        = (void *) FPU_Handler,         /* 61 Floating Point Unit */
    .pvReserved62          = static_cast<void *>(nullptr), /* 62 Reserved */
    .pfnRSWDT_Handler      = (void *) RSWDT_Handler,       /* 63 Reinforced Safety Watchdog Timer */
    .pfnCCW_Handler        = (void *) CCW_Handler,         /* 64 System Control Block */
    .pfnCCF_Handler        = (void *) CCF_Handler,         /* 65 System Control Block */
    .pfnGMAC_Q1_Handler    = (void *) GMAC_Q1_Handler,     /* 66 Gigabit Ethernet MAC */
    .pfnGMAC_Q2_Handler    = (void *) GMAC_Q2_Handler,     /* 67 Gigabit Ethernet MAC */
    .pfnIXC_Handler        = (void *) IXC_Handler,         /* 68 Floating Point Unit */
    .pfnI2SC0_Handler      = (void *) I2SC0_Handler,       /* 69 Inter-IC Sound Controller */
    .pfnI2SC1_Handler      = (void *) I2SC1_Handler,       /* 70 Inter-IC Sound Controller */
    .pfnGMAC_Q3_Handler    = (void *) GMAC_Q3_Handler,     /* 71 Gigabit Ethernet MAC */
    .pfnGMAC_Q4_Handler    = (void *) GMAC_Q4_Handler,     /* 72 Gigabit Ethernet MAC */
    .pfnGMAC_Q5_Handler    = (void *) GMAC_Q5_Handler      /* 73 Gigabit Ethernet MAC */
};


static inline void tcm_disable() {
    __asm volatile ("dsb"); // Data Synchronization Barrier
    __asm volatile ("isb"); // Instruction Synchronization Barrier
    SCB->ITCMCR &= ~(uint32_t)(1UL);
    SCB->DTCMCR &= ~(uint32_t) SCB_DTCMCR_EN_Msk;
    __asm volatile ("dsb"); // Data Synchronization Barrier
    __asm volatile ("isb"); // Instruction Synchronization Barrier
}

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
extern "C" void Reset_Handler() {
    uint32_t *pSrc, *pDest;

    WDT_REGS->WDT_MR = WDT_MR_WDDIS_Msk; // Disable the watchdog
    RSWDT_REGS->RSWDT_MR = RSWDT_MR_WDDIS_Msk; // Disable RSWDT
    //
    // SCB_DisableICache();
    // SCB_DisableDCache();

    /* Initialize the relocate segment */
    pSrc  = &_etext;
    pDest = &_srelocate;

    if (pSrc != pDest) {
        for (; pDest < &_erelocate;) {
            *pDest++ = *pSrc++;
        }
    }

    /* Clear the zero segment */
    for (pDest = &_szero; pDest < &_ezero;) {
        *pDest++ = 0;
    }

    // /* TCM Configuration */
    // EFC_REGS->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_CGPB
    //                  | EEFC_FCR_FARG(8));
    // EFC_REGS->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_CGPB
    //                  | EEFC_FCR_FARG(7));
    // tcm_disable();

    /* Set the vector table base address */
    pSrc      = (uint32_t *) &_sfixed;
    SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

    /* Enable the UsageFault_Handler */
    SCB->SHCSR = SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk;

    /* Call the optional application-provided _on_reset() function. */
    if (_on_reset) {
        _on_reset();
    }


    /* Initialize the C library */
    __libc_init_array();

    /* Call the optional application-provided _on_bootstrap() function. */
    if (_on_bootstrap) {
        _on_bootstrap();
    }

    /* Branch to main function */
    main();

    /* Call the optional application-provided _on_exit() function. */
    if (_on_exit) {
        _on_exit();
    } else {
        /* Infinite loop */
        while (true);
    }
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */

static volatile uint32_t cfsr = 0;
static volatile uint32_t hfsr = 0;
static volatile uint32_t abfsr = 0;
static volatile uint32_t mmfar = 0;
static volatile uint32_t bfar = 0;
static volatile uint32_t frame_r0  = 0;
static volatile uint32_t frame_r1  = 0;
static volatile uint32_t frame_r2  = 0;
static volatile uint32_t frame_r3  = 0;
static volatile uint32_t frame_r12 = 0;
static volatile uint32_t frame_lr  = 0;
static volatile uint32_t frame_pc  = 0;
static volatile uint32_t frame_psr = 0;

extern "C" void capture_stack_frame(uint32_t *frame) {
    cfsr      = SCB->CFSR;
    hfsr      = SCB->HFSR;
    abfsr     = SCB->ABFSR;
    mmfar     = SCB->MMFAR;
    bfar      = SCB->BFAR;
    frame_r0  = frame[0];
    frame_r1  = frame[1];
    frame_r2  = frame[2];
    frame_r3  = frame[3];
    frame_r12 = frame[4];
    frame_lr  = frame[5];
    frame_pc  = frame[6];
    frame_psr = frame[7];
    __asm volatile ("bkpt #0");
    while (true) {
        __NOP();
    }
}

__attribute__((naked))
void HardFault_Handler() {
    __asm volatile
    (
        "tst lr, #4    \n"
        "ite eq        \n"
        "mrseq r0, msp \n"
        "mrsne r0, psp \n"
        "b capture_stack_frame \n"
    );
}
__attribute__((naked))
void BusFault_Handler() {
    __asm volatile
    (
        "tst lr, #4    \n"
        "ite eq        \n"
        "mrseq r0, msp \n"
        "mrsne r0, psp \n"
        "b capture_stack_frame \n"
    );
}

void Dummy_Handler() {
    cfsr  = SCB->CFSR;
    hfsr  = SCB->HFSR;
    abfsr = SCB->ABFSR;
    mmfar = SCB->MMFAR;
    bfar  = SCB->BFAR;
    __asm volatile ("bkpt #0");
    while (true) {
        __NOP();
    }
}

void NonMaskableInt_Handler() {
    cfsr  = SCB->CFSR;
    hfsr  = SCB->HFSR;
    abfsr = SCB->ABFSR;
    mmfar = SCB->MMFAR;
    bfar  = SCB->BFAR;
    __asm volatile ("bkpt #0");
    while (true) {
        __NOP();
    }
}

void MemoryManagement_Handler() {
    cfsr  = SCB->CFSR;
    hfsr  = SCB->HFSR;
    abfsr = SCB->ABFSR;
    mmfar = SCB->MMFAR;
    bfar  = SCB->BFAR;
    __asm volatile ("bkpt #0");
    while (true) {
        __NOP();
    }
}

void UsageFault_Handler() {
    cfsr  = SCB->CFSR;
    hfsr  = SCB->HFSR;
    mmfar = SCB->MMFAR;
    bfar  = SCB->BFAR;
    __asm volatile ("bkpt #0");
    while (true) {
        __NOP();
    }
}

void DebugMonitor_Handler() {
    cfsr  = SCB->CFSR;
    hfsr  = SCB->HFSR;
    mmfar = SCB->MMFAR;
    bfar  = SCB->BFAR;
    __asm volatile ("bkpt #0");
    while (true) {
        __NOP();
    }
}
//
// void Dummy_SysTick_Handler() {
//     __NOP();
// }

// void SVCall_Handler() {
//     cfsr  = SCB->CFSR;
//     hfsr  = SCB->HFSR;
//     mmfar = SCB->MMFAR;
//     bfar  = SCB->BFAR;
//     __asm volatile ("bkpt #0");
//     while (true) {
//         __NOP();
//     }
// }

// void PendSV_Handler() {
//     cfsr  = SCB->CFSR;
//     hfsr  = SCB->HFSR;
//     mmfar = SCB->MMFAR;
//     bfar  = SCB->BFAR;
//     __asm volatile ("bkpt #0");
//     while (true) {
//         __NOP();
//     }
// }
