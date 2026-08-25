//
// Created by aeols on 2026-08-24.
//

#include "SamV71_UART.h"

namespace SamV71 {
    void SamV71_UART::initialize() {
    }

    bool SamV71_UART::has_input() {
        return false;
    }

    bool SamV71_UART::for_each_input(Optional_user, Optional_func) {
        return false;
    }

    bool SamV71_UART::is_ready() {
        return false;
    }

    int SamV71_UART::print(const char *ptr, int len) {
        return 0;
    }

    int SamV71_UART::put(uint8_t c) {
        return 0;
    }

    bool SamV71_UART::get(uint8_t *p_data) {
        return false;
    }
} // SamV71
