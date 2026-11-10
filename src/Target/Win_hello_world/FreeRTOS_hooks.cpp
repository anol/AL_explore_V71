//
// Created by aeols on 2026-09-08.
//
// FreeRTOS application hooks for the Windows simulator port.

#include <cstdio>
#include <cstdlib>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
}

namespace {
    [[noreturn]] void stop_here(const char *reason) {
        std::fprintf(stderr, "\r\n<> FreeRTOS: %s <>\r\n", reason);
        std::fflush(stderr);
        if (IsDebuggerPresent()) {
            DebugBreak();
        }
        std::abort();
    }
}

extern "C" {
void vAssertCalled(const char *pcFile, const unsigned long ulLine) {
    std::fprintf(stderr, "\r\n<> configASSERT failed: %s:%lu <>\r\n", pcFile, ulLine);
    stop_here("assertion");
}

void vApplicationMallocFailedHook(void) {
    stop_here("malloc failed (increase configTOTAL_HEAP_SIZE)");
}

static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   configSTACK_DEPTH_TYPE *puxIdleTaskStackSize) {
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *puxIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    configSTACK_DEPTH_TYPE *puxTimerTaskStackSize) {
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *puxTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
}
