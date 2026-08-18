#pragma once

#include "Interface/Board_utility_interface.h"
#include "component/flexcom/Flexcom_UART.h"
#include "Filestore/File_repository.h"
#include "GRMU_platform/GRMU_memory_manager.h"
#include "component/mcan/SAMRH71_CAN_peripheral.h"
#include "Bus_manager/Bus_manager.h"
#include "NORM_pin_manager.h"

class SamV71_board_support : public Abstract_board {
    using Pin = Abstract::IO_pin_interface;
    Flexcom_UART the_UART;

public:
    SamV71_board_support();

    bool initialize() override;

    Filestore &get_filestore() override { return the_filestore; }

    Pin &get_pin(Pin::Pin_id id) override {
        return Platform::NORM_pin_manager::get_pin(id);
    }

    UART_interface &get_UART() override { return the_UART; }

    Timer_interface *get_optional_timer(Timer_interface::Timer_function) override { return nullptr; };

    Pin *get_optional_pin(Pin::Pin_id) override { return nullptr; };

};
