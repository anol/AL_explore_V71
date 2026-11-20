//
// Created by aeols on 12.08.2026.
//

#include "Hello_AstraLEO_EM.h"

import Application.Hello_AstraLEO;

import Board.AstraLEO_EM_board;

namespace
{
    Board::AstraLEO_EM_board the_board{};
    Application::Hello_AstraLEO the_application{the_board};
    Target::Hello_AstraLEO_EM the_target{the_application, the_board};
}

int main()
{
    the_target.initialize();
    the_target.run();
}

namespace Target
{
} // Target

