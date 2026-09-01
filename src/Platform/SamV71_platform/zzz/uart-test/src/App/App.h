//
// Created by anolsen on 19.08.2019.
//

#ifndef UART_TEST_APP_H
#define UART_TEST_APP_H

#include "Consol.h"
#include "Labio.h"

class App {
public:
    App();

    void run();

private:
    int loop_body(int delay);

private:
    Consol consol;
    Labio labio;
};

#endif //UART_TEST_APP_H
