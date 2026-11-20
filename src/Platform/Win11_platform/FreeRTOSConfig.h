/*
 * FreeRTOSConfig.h — Windows host (FreeRTOS Windows simulator port, portable/MSVC-MingW), FreeRTOS Kernel V11
 * Toolchain: MinGW-w64 GCC
 *
 * Kept as close as possible to the SAMV71 configuration so the application behaves the same; only the
 * hardware-specific parts (clock, NVIC priorities, FPU/MPU, handler names) are dropped.
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

/* ---------------------------------------------------------------------
 * Scheduler
 * ------------------------------------------------------------------- */
#define configUSE_PREEMPTION                    1
#define configUSE_TIME_SLICING                  1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE                 0
#define configTICK_RATE_HZ                      ( 1000 )   /* Windows timer resolution is ~1 ms at best */
#define configMAX_PRIORITIES                    ( 7 )
#define configMINIMAL_STACK_SIZE                ( 128 )    /* words; the port runs each task on a Windows thread */
#define configMAX_TASK_NAME_LEN                 ( 16 )
#define configTICK_TYPE_WIDTH_IN_BITS           TICK_TYPE_WIDTH_32_BITS
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
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   3

/* ---------------------------------------------------------------------
 * Memory allocation
 * ------------------------------------------------------------------- */
#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configTOTAL_HEAP_SIZE                   ( 512 * 1024 )  /* StackType_t is 8 bytes on x64 */
#define configAPPLICATION_ALLOCATED_HEAP        0

/* ---------------------------------------------------------------------
 * Hooks
 * ------------------------------------------------------------------- */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configUSE_MALLOC_FAILED_HOOK            1
#define configCHECK_FOR_STACK_OVERFLOW          0   /* tasks run on Windows thread stacks, not the FreeRTOS one */

/* ---------------------------------------------------------------------
 * Run-time / debug stats
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
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

/* ---------------------------------------------------------------------
 * "Interrupt" priorities. The Windows port has no NVIC; these only exist so platform-independent code that
 * refers to them (e.g. SPI IRQ priority definitions) still compiles.
 * ------------------------------------------------------------------- */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY      7
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

/* ---------------------------------------------------------------------
 * API inclusion
 * ------------------------------------------------------------------- */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_xTaskAbortDelay                 1
#define INCLUDE_xTaskGetHandle                  0

/* ---------------------------------------------------------------------
 * configASSERT — report file/line and stop (see FreeRTOS_hooks.cpp).
 * ------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif
void vAssertCalled( const char * pcFile, unsigned long ulLine );
#ifdef __cplusplus
}
#endif

#define configASSERT( x ) \
    if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )

#endif /* FREERTOS_CONFIG_H */
