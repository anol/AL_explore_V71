//
// Created by anolsen on 19.08.2019.
//

#ifndef UART_TEST_APP_H
#define UART_TEST_APP_H

#include <IDE3466.h>
#include <gpio/GPIO_pin.h>
#include "Consol.h"
#include "Labio.h"
#include "Command_handler.h"

class App {
public:
    App();

    void run();

private:
    void initialize();

    int loop_body(int delay);

private:
    GPIO_pin warning_led ;
    GPIO_pin heartbeat_led ;
    Consol m_consol;
    IDE3466 m_detector;
    Command_handler m_handler;
    Labio m_labio;
};

#endif //UART_TEST_APP_H
