//
// Created by aeols on 12.08.2026.
//

#include "Win_hello_world.h"

import Application.Hello_world;
import Board.Win11_board;

namespace
{
    Board::Win11_board the_board{};
    Application::Hello_world the_application{the_board};
    Target::Windows_hello_world the_target{the_application, the_board};
}

int main()
{
    the_target.initialize();
    the_target.run();
}
