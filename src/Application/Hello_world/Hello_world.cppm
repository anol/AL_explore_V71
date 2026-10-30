//
// Created by aeols on 12.08.2026.
//

module;
#include "Persistent_parameter_id.h"
#include <cstdint>
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"

export module Application.Hello_world;
import Type.Abstract_application;
import Type.Abstract_board;
import Application.Detector;
import Application.Service_provider;
import Application.SpectraNode_console;
import Support.Heartbeat_task;
import Domain.IO_pins;

export namespace Application
{
    class Hello_world : public Abstract::Abstract_application
    {
        bool the_toggle_flag{};

        Abstract::Abstract_UART& use_UART;
        Abstract::Abstract_IO_pin& use_LED0;
        Abstract::Abstract_IO_pin& use_LED1;

        Detector the_detector;
        Service_provider the_service_provider{use_UART, the_detector};
        SpectraNode_console the_console{use_UART, the_service_provider};
        Support::Heartbeat_task the_heartbeat_task{use_LED0, use_LED1};
        uint32_t the_background_count{};

    public:
        explicit Hello_world(Abstract::Abstract_board& board)
            : use_UART(board.get_UART()), use_LED0(board.get_pin(Domain::Pin_LED0)), use_LED1(board.get_pin(Domain::Pin_LED1)),
              the_detector(board)
        {
        }

        void initialize() override;

        void run() override;
    };
} // Application
