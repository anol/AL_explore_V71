//
// Created by aeols on 2026-09-10.
//

module Support.Console_support;

namespace Support
{
    void Console_support::initialize()
    {
        the_stdio.initialize();
        the_receiver.initialize();
        the_transmitter.initialize();
    }
} // Support
