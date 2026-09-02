//
// Created by anolsen on 23.09.2019.
//

#ifndef SPI_TEST_COMMAND_HANDLER_H
#define SPI_TEST_COMMAND_HANDLER_H


#include <IDE3466.h>
#include "New_CLI.h"
#include "Consol.h"

class Command_handler {

public:
    Command_handler(Consol &consol, IDE3466 &detector);

    void execute_command(New_CLI::parse_result result);

    void initialize();

    void transaction_finish();

private:
    Consol &r_consol;
    IDE3466 &r_detector;

    void do_test();

    void do_register_dump();
};


#endif //SPI_TEST_COMMAND_HANDLER_H
