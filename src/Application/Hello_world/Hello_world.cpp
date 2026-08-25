//
// Created by aeols on 12.08.2026.
//

#include <stdio.h>
#include "Hello_world.h"

namespace Application {
    void Hello_world::initialize() {
        printf("Hello_world::initialize\r\n");
    }

    void Hello_world::run() {
        printf("Hello_world::run\r\n");
        while (true);
    }
} // Application
