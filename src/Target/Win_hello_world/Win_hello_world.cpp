//
// Created by aeols on 12.08.2026.
//

#include "Win_hello_world.h"

//#include "Clock_thread.h"
#include "Hello_world.h"
#include "Board/Win11/Win11.h"

//static Windows::Clock_thread the_clock_thread{};
static Application::Hello_world the_application{};
static Board::Win11 the_board{};
static Target::Windows_hello_world the_target{the_application, the_board};

int main() {
    the_target.initialize();
//    the_clock_thread.run();
    the_target.run();
}

namespace Target {
} // Target
