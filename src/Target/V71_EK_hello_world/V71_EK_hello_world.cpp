//
// Created by aeols on 12.08.2026.
//

#include "V71_EK_hello_world.h"

#include "Hello_world.h"

#include "V71_EK_board.h"

namespace
{
    Board::V71_EK_board the_board{};
    Application::Hello_world the_application{the_board};
    Target::V71_EK_hello_world the_target{the_application, the_board};
}

int main()
{
    the_target.initialize();
    the_target.run();
}

namespace Target
{
} // Target

