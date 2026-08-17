// Minimal stand-in for the SAMV71 CMSIS device header, providing only the
// symbols SamV71EthernetMac.h/.cpp reference, with bit positions matching
// what was verified against Microchip's public docs / ASF gmac.h (see
// README.md). This is NOT a substitute for the real CMSIS-SAMV71 pack --
// it exists purely so this repo's compile-verification step can catch
// C++-level mistakes without a real SAMV71 toolchain installed.
#pragma once
#include <cstdint>

struct GmacSaEntry {
    volatile uint32_t GMAC_SAB;
    volatile uint32_t GMAC_SAT;
};

struct Gmac {
    volatile uint32_t GMAC_NCR;
    volatile uint32_t GMAC_NCFGR;
    volatile uint32_t GMAC_NSR;
    volatile uint32_t GMAC_UR;
    volatile uint32_t GMAC_DCFGR;
    volatile uint32_t GMAC_TSR;
    volatile uint32_t GMAC_RBQB;
    volatile uint32_t GMAC_TBQB;
    volatile uint32_t GMAC_RSR;
    volatile uint32_t GMAC_ISR;
    volatile uint32_t GMAC_IER;
    volatile uint32_t GMAC_IDR;
    volatile uint32_t GMAC_IMR;
    volatile uint32_t GMAC_MAN;
    GmacSaEntry GMAC_SA[4];
};

struct Pmc {
    volatile uint32_t PMC_PCER1;
};

inline Gmac gGmacInstance{};
inline Pmc gPmcInstance{};
#define GMAC (&gGmacInstance)
#define PMC (&gPmcInstance)

constexpr uint32_t ID_GMAC = 39;

constexpr uint32_t GMAC_NCR_RXEN   = (1u << 2);
constexpr uint32_t GMAC_NCR_TXEN   = (1u << 3);
constexpr uint32_t GMAC_NCR_MPE    = (1u << 4);
constexpr uint32_t GMAC_NCR_TSTART = (1u << 9);

constexpr uint32_t GMAC_NCFGR_SPD   = (1u << 0);
constexpr uint32_t GMAC_NCFGR_FD    = (1u << 1);
constexpr uint32_t GMAC_NCFGR_MAXFS = (1u << 8);

constexpr uint32_t GMAC_DCFGR_TXPBMS      = (1u << 10);
constexpr uint32_t GMAC_DCFGR_RXBMS_FULL  = (0x3u << 8);
constexpr uint32_t GMAC_DCFGR_FBLDO_INCR4 = (0x4u << 0);

constexpr uint32_t GMAC_UR_RMII = (1u << 0);

constexpr uint32_t GMAC_ISR_RCOMP = (1u << 1);
constexpr uint32_t GMAC_ISR_RXUBR = (1u << 2);
constexpr uint32_t GMAC_ISR_ROVR  = (1u << 10);

constexpr uint32_t GMAC_NSR_IDLE = (1u << 2);

using IRQn_Type = int;
constexpr IRQn_Type GMAC_IRQn = 40;
inline void NVIC_EnableIRQ(IRQn_Type) {}
inline void NVIC_DisableIRQ(IRQn_Type) {}
inline void NVIC_ClearPendingIRQ(IRQn_Type) {}

#define __DSB() do {} while (0)

#define __DCACHE_PRESENT 1
inline void SCB_CleanDCache_by_Addr(uint32_t*, int32_t) {}
inline void SCB_InvalidateDCache_by_Addr(uint32_t*, int32_t) {}
