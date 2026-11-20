//
// Created by aeols on 12.08.2026.
//

#include "Hello_AstraLEO_Win11.h"

import Application.Hello_AstraLEO;
import Board.Win11_board;

namespace
{
    Board::Win11_board the_board{};
    Application::Hello_AstraLEO the_application{the_board};
    Target::Hello_AstraLEO_Win11 the_target{the_application, the_board};
}

int main()
{
    the_target.initialize();
    the_target.run();
}
