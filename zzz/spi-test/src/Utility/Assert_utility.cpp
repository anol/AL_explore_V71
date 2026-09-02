//
// Created by anolsen on 17.09.2019.
//

#include "Assert_utility.h"

void special_assert(bool condition) {
    if (!condition) {
        __asm__("bkpt 0");
    }
}