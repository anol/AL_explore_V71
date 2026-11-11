module;
#include <atomic>
#include <cstdint>
#include <cstdio>

#include "Mockup_host.h"

module Platform.Mockup_UART;

namespace Mockup {
    void Mockup_UART::initialize() {
        setvbuf(stdout, nullptr, _IONBF, 0);
        if (!the_started_flag) {
            the_started_flag = Mockup_host::start_stdin_reader(
                [](void *user, const uint8_t data) { static_cast<Mockup_UART *>(user)->on_input(data); }, this);
        }
    }

    void Mockup_UART::on_input(const uint8_t data) {
        const auto head = the_head.load(std::memory_order_relaxed);
        const auto next = (head + 1) % RX_buffer_size;
        if (next == the_tail.load(std::memory_order_acquire)) {
            the_overflow_count.fetch_add(1, std::memory_order_relaxed);
            return;
        }
        the_RX_buffer[head] = data;
        the_head.store(next, std::memory_order_release);
    }

    void Mockup_UART::inject(const char *text) {
        while (text && *text) {
            on_input(static_cast<uint8_t>(*text++));
        }
    }

    bool Mockup_UART::pop(uint8_t &data) {
        const auto tail = the_tail.load(std::memory_order_relaxed);
        if (tail == the_head.load(std::memory_order_acquire)) {
            return false;
        }
        data = the_RX_buffer[tail];
        the_tail.store((tail + 1) % RX_buffer_size, std::memory_order_release);
        return true;
    }

    void Mockup_UART::wait_a_little() {
        Mockup_host::task_sleep(Poll_millis);
    }

    Status_code Mockup_UART::get(uint8_t *data) {
        if (data && pop(*data)) {
            return Status_code::Success();
        }
        wait_a_little();
        return Status_code::Failure();
    }

    Status_code Mockup_UART::put(const uint8_t data) {
        return Status_code(std::fputc(data, stdout) != EOF);
    }

    int Mockup_UART::print(const char *data, const int len) {
        if (data == nullptr || len <= 0) {
            return 0;
        }
        return static_cast<int>(std::fwrite(data, 1, static_cast<size_t>(len), stdout));
    }

    bool Mockup_UART::for_each_input(Optional_user user, Optional_func func) {
        // The Windows console already echoes typed characters (cooked mode), so the echo flag
        // is kept for interface compatibility only.
        uint8_t data;
        while (pop(data)) {
            if (func) {
                func(user, data);
            }
        }
        return true;
    }
} // Mockup
