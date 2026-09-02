//
// Created by anolsen on 23.09.2019.
//

#include <cstdint>
#include <cstring>
#include "New_CLI.h"
#include "Consol.h"
#include "Command_handler.h"


Command_handler::Command_handler(Consol &consol, IDE3466 &detector) :
        r_consol(consol),
        r_detector(detector) {}

void Command_handler::initialize() {

}

void Command_handler::execute_command(New_CLI::parse_result result) {
    New_CLI::print_result(&result);
    switch (result.action) {
        case NONE:
            break;
        case HELP:
            break;
        case INIT:
            r_detector.write_all_registers();
            break;
        case REGISTER_DUMP:
            do_register_dump();
            break;
        case TEST:
            do_test();
            break;
        default:
            break;
    }

}

static void my_transaction_finish(void *p_user) {
    ((Command_handler *) p_user)->transaction_finish();
}

void Command_handler::transaction_finish() {
    uint32_t register_value = r_detector.get_register_bank(true).global_configuration.get_rx(0);
    Consol::global_printf("=0x%08X\r\n\r\n", register_value);
}

void Command_handler::do_test() {
    r_consol.print("\r\nTest ...\r\n");
    SPI_proxy::SPI_transaction transaction{};
    transaction.p_user = this;
    transaction.finish = my_transaction_finish;
    r_detector.get_register_bank(true).global_configuration.read(r_detector.get_proxy(), transaction);
}

static void my_print_register_value(void *p_user, int reg_number, uint32_t data, const char *name) {
    if (0 == reg_number) {
        Consol::global_printf("\r\n%s\r\n", name);
    }
    Consol::global_printf("%4d = 0x%06X\r\n", reg_number, data);
}

void Command_handler::do_register_dump() {
    r_consol.print("\r\nRegister dump\r\n");
    r_consol.print("\r\n\r\nRead all registers ...\r\n");
    r_detector.read_all_registers();
    r_detector.get_register_bank(true).for_each_rx(this, my_print_register_value);
    r_consol.print("\r\n\r\n... finish.\r\n");
}
