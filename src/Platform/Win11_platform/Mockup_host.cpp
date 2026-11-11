#include "Mockup_host.h"

#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

namespace Mockup_host {
    namespace {
        struct Reader {
            Input_callback the_callback;
            void *the_user;
        };

        Reader the_reader{};

        DWORD WINAPI reader_thread(LPVOID) {
            // A plain Windows thread: it must never call the FreeRTOS API.
            for (int data = std::getchar(); data != EOF; data = std::getchar()) {
                the_reader.the_callback(the_reader.the_user, static_cast<uint8_t>(data));
            }
            return 0;
        }
    }

    bool start_stdin_reader(const Input_callback callback, void *user) {
        if (callback == nullptr || the_reader.the_callback != nullptr) {
            return false;
        }
        the_reader = {callback, user};
        HANDLE thread = CreateThread(nullptr, 0, reader_thread, nullptr, 0, nullptr);
        if (thread == nullptr) {
            return false;
        }
        CloseHandle(thread);
        return true;
    }

    void task_sleep(const uint32_t millis) {
        if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) {
            const TickType_t ticks = pdMS_TO_TICKS(millis);
            vTaskDelay(ticks ? ticks : 1);
        }
    }
}
