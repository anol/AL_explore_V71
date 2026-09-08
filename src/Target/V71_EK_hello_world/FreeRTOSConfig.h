/*
 * FreeRTOSConfig.h — SAMV71 (Cortex-M7), FreeRTOS Kernel V11.3.1
 * Toolchain: arm-none-eabi-gcc, portable/GCC/ARM_CM7/r0p1
 *
 * Fill in the two TODOs (clock source, heap size) for your board setup,
 * everything else is safe as a starting point for bring-up.
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Pull in the real clock value from your CMSIS/clock-init code rather than
 * hardcoding it, so it stays correct if you change the PLL config later. */
#include <stdint.h>
extern uint32_t SystemCoreClock;   /* Updated in SamV71_clock.cpp */

/* ---------------------------------------------------------------------
 * Scheduler
 * ------------------------------------------------------------------- */
#define configUSE_PREEMPTION                   1
#define configUSE_TIME_SLICING                 1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#define configUSE_TICKLESS_IDLE                 0   /* enable later once base RTOS is verified */

#define configCPU_CLOCK_HZ                     ( SystemCoreClock )
#define configTICK_RATE_HZ                     ( 1000 )
#define configMAX_PRIORITIES                   ( 7 )
#define configMINIMAL_STACK_SIZE                ( 128 )   /* words; idle/timer task stack */
#define configMAX_TASK_NAME_LEN                 ( 16 )
#define configUSE_16_BIT_TICKS                  0

#define configIDLE_SHOULD_YIELD                 1

/* ---------------------------------------------------------------------
 * Synchronization primitives
 * ------------------------------------------------------------------- */
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configUSE_QUEUE_SETS                    1
#define configQUEUE_REGISTRY_SIZE               8
#define configUSE_TASK_NOTIFICATIONS            1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES    3

/* ---------------------------------------------------------------------
 * Memory allocation
 * ------------------------------------------------------------------- */
#define configSUPPORT_STATIC_ALLOCATION         0
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configTOTAL_HEAP_SIZE                   ( 64 * 1024 )  /* TODO: budget against
                                                    your SAMV71 part's actual SRAM size
                                                    and whatever you reserve for
                                                    non-cacheable DMA buffers */
#define configAPPLICATION_ALLOCATED_HEAP        0

/* heap_4.c — pair this config with portable/MemMang/heap_4.c */

/* ---------------------------------------------------------------------
 * Hooks
 * ------------------------------------------------------------------- */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configUSE_MALLOC_FAILED_HOOK            1
#define configCHECK_FOR_STACK_OVERFLOW           2   /* method 2: canary + SP check, keep
                                                    on until bring-up is solid, then
                                                    reassess the cycle cost */

/* ---------------------------------------------------------------------
 * Run-time / debug stats (optional, cheap to leave on during bring-up)
 * ------------------------------------------------------------------- */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    1

/* ---------------------------------------------------------------------
 * Co-routines / software timers
 * ------------------------------------------------------------------- */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         2

#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                 10
#define configTIMER_TASK_STACK_DEPTH             configMINIMAL_STACK_SIZE

/* ---------------------------------------------------------------------
 * Cortex-M7 / NVIC interrupt priority configuration
 *
 * SAMV71 implements 4 priority bits (16 levels: 0..15, 0 = highest).
 * Priority-grouping MUST be all-preemption / no-subpriority
 * (NVIC_SetPriorityGrouping(0)) — verify this explicitly at startup,
 * don't assume the CMSIS default is already right for your part/SDK.
 * ------------------------------------------------------------------- */
// #define configPRIO_BITS                         4
#define configPRIO_BITS                         3

/* Lowest priority = numerically largest value the hardware supports. */
//#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY  15
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY  7

/* Highest priority from which FreeRTOS API calls are allowed
 * (xxxFromISR functions). Any ISR at a NUMERICALLY LOWER priority than
 * this must not touch the RTOS API at all. Leave headroom above this
 * for any truly hard-real-time ISR you want completely outside
 * scheduler control. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY  5

/* Shifted values consumed by the Cortex-M port layer — derive them,
 * don't hand-compute, so a configPRIO_BITS change can't silently
 * desync these. */
#define configKERNEL_INTERRUPT_PRIORITY \
    ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* ---------------------------------------------------------------------
 * FPU (Cortex-M7 has one; r0p1 port does lazy stacking of FPU context)
 * ------------------------------------------------------------------- */
#define configENABLE_FPU                        1
#define configENABLE_MPU                        0   /* set 1 only if moving to FreeRTOS-MPU */
#define configENABLE_TRUSTZONE                  0   /* N/A on Cortex-M7 */

/* ---------------------------------------------------------------------
 * API inclusion
 * ------------------------------------------------------------------- */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState           1
#define INCLUDE_xTaskGetCurrentTaskHandle         1
#define INCLUDE_uxTaskGetStackHighWaterMark        1
#define INCLUDE_xTaskGetIdleTaskHandle            0
#define INCLUDE_eTaskGetState                    1
#define INCLUDE_xTimerPendFunctionCall            1
#define INCLUDE_xTaskAbortDelay                  1
#define INCLUDE_xTaskGetHandle                   0

/* ---------------------------------------------------------------------
 * configASSERT — wire this to something that actually halts and is
 * visible in the debugger (not a silent no-op) while you're bringing
 * this up. A hardfault from a bad priority config is otherwise very
 * hard to tell apart from any other hardfault.
 * ------------------------------------------------------------------- */
void vAssertCalled( const char * pcFile, unsigned long ulLine );
#define configASSERT( x ) \
    if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )

/* Standard Cortex-M handler names to route to the FreeRTOS port.
 * Make sure your startup file's vector table entries for SVC, PendSV,
 * and SysTick point at these (remove any bare-metal SysTick_Handler /
 * SVC_Handler you already have — the RTOS owns them now). */
#define vPortSVCHandler        SVCall_Handler
#define xPortPendSVHandler     PendSV_Handler
#define xPortSysTickHandler    SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
