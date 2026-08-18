#include "SamV71_board_support.h"
#include "SamV71_pin_manager.h"

const Flexcom_UART::Flexcom_UART_definition UART_definition = {
    1, ID_FLEXCOM1, FLEXCOM1_IRQn, FLEXCOM1_REGS
    //            SAMRH71_GPIO::GPIO_PORTF, 29, SAMRH71_GPIO::PERIPHERAL_MODE_MUX_A,
    //            SAMRH71_GPIO::GPIO_PORTF, 30, SAMRH71_GPIO::PERIPHERAL_MODE_MUX_A
};

enum {
    UART_bitrate = 115'200,
};

SamV71_board_support::SamV71_board_support()
    : Abstract::Abstract_board(),
      the_UART(UART_definition) {
}

bool SamV71_board_support::initialize() {
    auto result = SamV71_pin_manager::initialize(Pin::Host_up);
    if (result.failed()) {
        Diagnostic::error_code(result);
    }
    NVIC_Initialize();
    the_UART.initialize(UART_bitrate);
    SamV71::SamV71_watchdog::check_watchdog();
    return success;
}

} // Platform
