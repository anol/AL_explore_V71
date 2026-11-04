//
// Created by aeols on 2026-11-04.
//

module;

#include "samv71q21b.h"

module Platform.SamV71_SPI:Driver;
import :Handler;

namespace SamV71 {
    static SamV71_SPI_handler *optional_handler_SPI0{};
}

extern "C" {
void ISR_SPI0() {
    if (auto *handler = SamV71::optional_handler_SPI0) {
        handler->ISR();
    } else {
        NVIC_DisableIRQ(IRQn::SPI0_IRQn);
        NVIC_ClearPendingIRQ(SPI0_IRQn);
    }
}
}

