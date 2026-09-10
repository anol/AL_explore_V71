//
// Created by aeols on 2026-09-10.
//

#include "Console_service.h"

namespace Console
{
    void Console_service::initialize()
    {
        the_receiver.initialize();
        the_transmitter.initialize();
    }
} // Console
