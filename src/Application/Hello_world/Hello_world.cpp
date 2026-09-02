//
// Created by aeols on 12.08.2026.
//

#include <stdio.h>
#include "Hello_world.h"

#include "IO_pins.h"

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
        auto toggle{false};
        printf("Hello_world::run\r\n");
        use_board.print_diagnostics();
        while (true)
        {
            delay(10'000'000);
            printf(".");
            toggle = !toggle;
            if (toggle)
            {
                use_board.get_pin(Dictionary::Pin_LED0).set();
                use_board.get_pin(Dictionary::Pin_LED1).clear();
            }else
            {
                use_board.get_pin(Dictionary::Pin_LED0).clear();
                use_board.get_pin(Dictionary::Pin_LED1).set();
            }
        }
    }
} // Application
