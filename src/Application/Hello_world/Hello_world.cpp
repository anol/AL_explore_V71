//
// Created by aeols on 12.08.2026.
//

#include <stdio.h>
#include "Hello_world.h"

namespace Application
{
    void Hello_world::initialize()
    {
        printf("Hello_world::initialize\r\n");
    }

    void Hello_world::delay(const int number_of_loops) const
    {
        for (int loop_counter = 0; loop_counter < number_of_loops; loop_counter++)
        {
            use_board.NOP();
        }
    }

    void Hello_world::run()
    {
        printf("Hello_world::run\r\n");
        use_board.print_diagnostics();
        while (true)
        {
            delay(1000000);
            printf(".");
        }
    }
} // Application
