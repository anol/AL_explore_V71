#ifndef CHIP_OSC_H_INCLUDED
#define CHIP_OSC_H_INCLUDED

#include <samv71q21b.h>
#include <board_definitions.h>
#include <pmc/pmc_driver.h>
#include <component/supc.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(BOARD_FREQ_SLCK_XTAL)
#  warning The board slow clock xtal frequency has not been defined.
#  define BOARD_FREQ_SLCK_XTAL      (32768UL)
#endif

#if !defined(BOARD_FREQ_SLCK_BYPASS)
#  warning The board slow clock bypass frequency has not been defined.
#  define BOARD_FREQ_SLCK_BYPASS    (32768UL)
#endif

#if !defined(BOARD_FREQ_MAINCK_XTAL)
#  warning The board main clock xtal frequency has not been defined.
#  define BOARD_FREQ_MAINCK_XTAL    (12000000UL)
#endif

#if !defined(BOARD_FREQ_MAINCK_BYPASS)
#  warning The board main clock bypass frequency has not been defined.
#  define BOARD_FREQ_MAINCK_BYPASS  (12000000UL)
#endif

#if !defined(BOARD_OSC_STARTUP_US)
#  warning The board main clock xtal startup time has not been defined.
#  define BOARD_OSC_STARTUP_US      (15625UL)
#endif

#define OSC_SLCK_32K_RC      0    //!< Internal 32kHz RC oscillator.
#define OSC_SLCK_32K_XTAL    1    //!< External 32kHz crystal oscillator.
#define OSC_SLCK_32K_BYPASS  2    //!< External 32kHz bypass oscillator.
#define OSC_MAINCK_4M_RC     3    //!< Internal 4MHz RC oscillator.
#define OSC_MAINCK_8M_RC     4    //!< Internal 8MHz RC oscillator.
#define OSC_MAINCK_12M_RC    5    //!< Internal 12MHz RC oscillator.
#define OSC_MAINCK_XTAL      6    //!< External crystal oscillator.
#define OSC_MAINCK_BYPASS    7    //!< External bypass oscillator.

#define OSC_SLCK_32K_RC_HZ      CHIP_FREQ_SLCK_RC         //!< Internal 32kHz RC oscillator.
#define OSC_SLCK_32K_XTAL_HZ    BOARD_FREQ_SLCK_XTAL      //!< External 32kHz crystal oscillator.
#define OSC_SLCK_32K_BYPASS_HZ  BOARD_FREQ_SLCK_BYPASS    //!< External 32kHz bypass oscillator.
#define OSC_MAINCK_4M_RC_HZ     CHIP_FREQ_MAINCK_RC_4MHZ  //!< Internal 4MHz RC oscillator.
#define OSC_MAINCK_8M_RC_HZ     CHIP_FREQ_MAINCK_RC_8MHZ  //!< Internal 8MHz RC oscillator.
#define OSC_MAINCK_12M_RC_HZ    CHIP_FREQ_MAINCK_RC_12MHZ //!< Internal 12MHz RC oscillator.
#define OSC_MAINCK_XTAL_HZ      BOARD_FREQ_MAINCK_XTAL    //!< External crystal oscillator.
#define OSC_MAINCK_BYPASS_HZ    BOARD_FREQ_MAINCK_BYPASS  //!< External bypass oscillator.

void osc_enable(uint32_t ul_id);

void osc_disable(uint32_t ul_id);

bool osc_is_ready(uint32_t ul_id);

uint32_t osc_get_rate(uint32_t ul_id);

void osc_wait_ready(uint8_t id);

#ifdef __cplusplus
}
#endif

#endif /* CHIP_OSC_H_INCLUDED */
