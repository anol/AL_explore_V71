//
// Created by aeols on 2026-09-10.
//

module;
#include "Abstract_service.h"
#include "Abstract_UART.h"
#include "Request_router.h"

export module Support.Console_service;
import :Console_receive_task;
import :Console_transmit_task;

namespace Console
{
    export class Console_service : public Abstract::Abstract_service
    {
        Console_receive_task the_receiver;
        Console_transmit_task the_transmitter{};

    public:
        Console_service(Abstract::Abstract_UART& UART, Request_router& router) :
            the_receiver(UART, router)
        {
        }

        void initialize() override;
    };
} // Console
