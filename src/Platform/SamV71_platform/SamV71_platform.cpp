module Platform.SamV71_platform;
import Platform.SamV71_clock;

#include "sam.h"

namespace SamV71 {
    void SamV71_platform::initialize() {
        enable_cache();
        initialize_matrix();
    }

    void SamV71_platform::enable_cache() {
        SCB_EnableICache();
        SCB_EnableDCache();
    }

    void SamV71_platform::initialize_matrix() {
        // Release the JTAG TDI pin
        MATRIX_REGS->CCFG_SYSIO |= CCFG_SYSIO_SYSIO4_Msk;
        // Release the JTAG ERASE pin
        MATRIX_REGS->CCFG_SYSIO |= CCFG_SYSIO_SYSIO12_Msk;
    }

} // SamV71
