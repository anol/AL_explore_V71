// Host (Windows / FreeRTOS) services for the mockups, kept out of module units because <windows.h> and the
// FreeRTOS headers do not mix well with C++20 modules in GCC.

#pragma once

#include <cstdint>

namespace Mockup_host {
    using Input_callback = void (*)(void *user, uint8_t data);

    /// Start a native (non-FreeRTOS) thread that reads stdin and calls callback(user, byte) for every byte.
    bool start_stdin_reader(Input_callback callback, void *user);

    /// Yield the calling FreeRTOS task for about the given time; no-op before the scheduler runs.
    void task_sleep(uint32_t millis);
}
