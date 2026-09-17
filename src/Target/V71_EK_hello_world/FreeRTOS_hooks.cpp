//
// Created by aeols on 2026-09-08.
//


extern "C" {
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "timers.h"
}

extern "C" {
void vAssertCalled(const char *pcFile, unsigned long ulLine) {
    volatile const char *  pcFileName   = pcFile; /* visible in debugger */
    volatile unsigned long ulLineNumber = ulLine;
    (void) pcFileName;
    (void) ulLineNumber;

    taskDISABLE_INTERRUPTS();

    /* Set a breakpoint here (or on __BKPT below) — when configASSERT
     * trips you want the debugger to land on this exact spot with
     * pcFile/ulLine populated so you can see what failed and where. */
    for (;;) {
        __asm volatile( "bkpt #0" );
    }
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    volatile TaskHandle_t xOverflowingTask = xTask;
    (void) xOverflowingTask;
    (void) pcTaskName; /* inspect in debugger: which task overflowed */

    taskDISABLE_INTERRUPTS();
    for (;;) {
        __asm volatile( "bkpt #0" );
    }
}

void vApplicationMallocFailedHook(void) {
    taskDISABLE_INTERRUPTS();
    for (;;) {
        __asm volatile( "bkpt #0" );
    }
}

static StaticTask_t xIdleTaskTCB;
static StackType_t  uxIdleTaskStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(
    StaticTask_t **ppxIdleTaskTCBBuffer,
    StackType_t ** ppxIdleTaskStackBuffer,
    uint32_t *     pulIdleTaskStackSize) {
    *ppxIdleTaskTCBBuffer   = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}

static StaticTask_t xTimerTaskTCB;
static StackType_t  uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory(
    StaticTask_t **ppxTimerTaskTCBBuffer,
    StackType_t ** ppxTimerTaskStackBuffer,
    uint32_t *     pulTimerTaskStackSize) {
    *ppxTimerTaskTCBBuffer   = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize   = configTIMER_TASK_STACK_DEPTH;
}
}
