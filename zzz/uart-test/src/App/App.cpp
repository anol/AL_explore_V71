//
// Created by anolsen on 19.08.2019.
//

#include <cstdint>
#include <board_definitions.h>
#include <board_initialization.h>
#include <command_parser.h>
#include <Assert_utility.h>

#include "App.h"

#define STRING_HEADER \
"-- NORM-DHU --\r\n" \
"-- UART-TEST --\r\n" \
"-- " BOARD_NAME " --\r\n" \
"-- Compiled: " __DATE__ " " __TIME__ " --\r\n"

enum {
    idle_delay = 1000, short_delay = 100, busy_loop_count = 10000
};

static void busy_delay(int nDelay) {
    volatile int ulLoop;
    while (0 < nDelay--) {
        for (ulLoop = busy_loop_count; 0 < ulLoop; ulLoop--) {
        }
    }
}

App::App() : consol(), labio(consol) {
    special_assert(&consol == &labio.m_consol);
}

void App::run() {
    int delay = idle_delay;
    board_init();
    consol.initialize(STRING_HEADER);
    while (true) {
        // NOTE: Endless loop warning here
        delay = loop_body(delay);
    }
}

int App::loop_body(int delay) {
    if (labio.receive_input()) {
        delay = short_delay;
    } else if (labio.execute_pending_commands()) {
        delay = short_delay;
    } else if (idle_delay > delay) {
        delay += short_delay;
    } else {
        consol.print_idler();
    }
    busy_delay(delay);
    return delay;
}
