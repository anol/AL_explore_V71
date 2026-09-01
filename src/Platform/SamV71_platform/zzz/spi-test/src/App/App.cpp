//
// Created by anolsen on 19.08.2019.
//

#include <stdint.h>
#include "board_definitions.h"
#include <board_initialization.h>
#include <Assert_utility.h>
#include <samv71q21b_pio.h>

#include "App.h"

#define STRING_HEADER \
"-- MPLAB --\r\n" \
"-- NORM Data Handling Unit --\r\n" \
"-- SPI-TEST w/ NORM CLI 1.0 --\r\n" \
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

App::App() :
        warning_led(PIO_PA23_IDX, true),
        heartbeat_led(PIO_PC9_IDX, true),
        m_consol(),
        m_detector(),
        m_handler(m_consol, m_detector),
        m_labio(m_consol, m_handler) {}

void App::initialize() {
    board_init();
    warning_led.enable();
    heartbeat_led.enable();
    m_consol.initialize(STRING_HEADER);
    m_detector.initialize();
    m_handler.initialize();
    warning_led.set();
}

void App::run() {
    int delay = idle_delay;
    initialize();
    while (true) {
        // NOTE: Endless loop warning here
        delay = loop_body(delay);
    }
}

int App::loop_body(int delay) {
    if (m_labio.receive_input()) {
        delay = short_delay;
    } else if (m_labio.execute_pending_commands()) {
        delay = short_delay;
    } else if (idle_delay > delay) {
        delay += short_delay;
    } else {
        m_consol.print_idler();
    }
    busy_delay(delay);
    heartbeat_led.toggle();
    return delay;
}
