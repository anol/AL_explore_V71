
#include <stdbool.h>
#include <samv71q21b.h>
#include <component/pmc.h>
#include <component/efc.h>

#include "sysclk.h"


#if defined(CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)
/**
 * \brief boolean signalling that the sysclk_init is done.
 */
uint32_t sysclk_initialized = 0;
#endif
uint32_t SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

static void SystemCoreClockUpdate(void) {
    /* Determine clock frequency according to clock register values */
    switch (PMC->PMC_MCKR & (uint32_t) PMC_MCKR_CSS_Msk) {
        case PMC_MCKR_CSS_SLOW_CLK: /* Slow clock */
            if (SUPC->SUPC_SR & SUPC_SR_OSCSEL) {
                SystemCoreClock = CHIP_FREQ_XTAL_32K;
            } else {
                SystemCoreClock = CHIP_FREQ_SLCK_RC;
            }
            break;
        case PMC_MCKR_CSS_MAIN_CLK: /* Main clock */
            if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) {
                SystemCoreClock = CHIP_FREQ_XTAL_12M;
            } else {
                SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;
                switch (PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk) {
                    case CKGR_MOR_MOSCRCF_4_MHz:
                        break;
                    case CKGR_MOR_MOSCRCF_8_MHz:
                        SystemCoreClock *= 2U;
                        break;
                    case CKGR_MOR_MOSCRCF_12_MHz:
                        SystemCoreClock *= 3U;
                        break;
                    default:
                        break;
                }
            }
            break;
        case PMC_MCKR_CSS_PLLA_CLK:    /* PLLA clock */
            if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) {
                SystemCoreClock = CHIP_FREQ_XTAL_12M;
            } else {
                SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;
                switch (PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk) {
                    case CKGR_MOR_MOSCRCF_4_MHz:
                        break;
                    case CKGR_MOR_MOSCRCF_8_MHz:
                        SystemCoreClock *= 2U;
                        break;
                    case CKGR_MOR_MOSCRCF_12_MHz:
                        SystemCoreClock *= 3U;
                        break;
                    default:
                        break;
                }
            }
            if ((uint32_t) (PMC->PMC_MCKR & (uint32_t) PMC_MCKR_CSS_Msk) == PMC_MCKR_CSS_PLLA_CLK) {
                SystemCoreClock *= ((((PMC->CKGR_PLLAR) & CKGR_PLLAR_MULA_Msk) >> CKGR_PLLAR_MULA_Pos) + 1U);
                SystemCoreClock /= ((((PMC->CKGR_PLLAR) & CKGR_PLLAR_DIVA_Msk) >> CKGR_PLLAR_DIVA_Pos));
            }
            break;
        default:
            break;
    }
    if ((PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) == PMC_MCKR_PRES_CLK_3) {
        SystemCoreClock /= 3U;
    } else {
        SystemCoreClock >>= ((PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) >> PMC_MCKR_PRES_Pos);
    }
}

void system_init_flash(uint32_t ul_clk) {
    /* Set FWS for embedded Flash access according to operating frequency */
    if (ul_clk < CHIP_FREQ_FWS_0) {
        EFC->EEFC_FMR = EEFC_FMR_FWS(0) | EEFC_FMR_CLOE;
    } else {
        if (ul_clk < CHIP_FREQ_FWS_1) {
            EFC->EEFC_FMR = EEFC_FMR_FWS(1) | EEFC_FMR_CLOE;
        } else {
            if (ul_clk < CHIP_FREQ_FWS_2) {
                EFC->EEFC_FMR = EEFC_FMR_FWS(2) | EEFC_FMR_CLOE;
            } else {
                if (ul_clk < CHIP_FREQ_FWS_3) {
                    EFC->EEFC_FMR = EEFC_FMR_FWS(3) | EEFC_FMR_CLOE;
                } else {
                    if (ul_clk < CHIP_FREQ_FWS_4) {
                        EFC->EEFC_FMR = EEFC_FMR_FWS(4) | EEFC_FMR_CLOE;
                    } else {
                        if (ul_clk < CHIP_FREQ_FWS_5) {
                            EFC->EEFC_FMR = EEFC_FMR_FWS(5) | EEFC_FMR_CLOE;
                        } else {
                            EFC->EEFC_FMR = EEFC_FMR_FWS(6) | EEFC_FMR_CLOE;
                        }
                    }
                }
            }
        }
    }
}

void sysclk_set_prescalers(uint32_t ul_pres) {
    pmc_mck_set_prescaler(ul_pres);
    SystemCoreClockUpdate();
}

void sysclk_set_source(uint32_t ul_src) {
    switch (ul_src) {
        case SYSCLK_SRC_SLCK_RC:
        case SYSCLK_SRC_SLCK_XTAL:
        case SYSCLK_SRC_SLCK_BYPASS:
            pmc_mck_set_source(PMC_MCKR_CSS_SLOW_CLK);
            break;
        case SYSCLK_SRC_MAINCK_4M_RC:
        case SYSCLK_SRC_MAINCK_8M_RC:
        case SYSCLK_SRC_MAINCK_12M_RC:
        case SYSCLK_SRC_MAINCK_XTAL:
        case SYSCLK_SRC_MAINCK_BYPASS:
            pmc_mck_set_source(PMC_MCKR_CSS_MAIN_CLK);
            break;
        case SYSCLK_SRC_PLLACK:
            pmc_mck_set_source(PMC_MCKR_CSS_PLLA_CLK);
            break;
        case SYSCLK_SRC_UPLLCK:
            pmc_mck_set_source(PMC_MCKR_CSS_UPLL_CLK);
            break;
    }
    SystemCoreClockUpdate();
}

#if defined(CONFIG_USBCLK_SOURCE) || defined(__DOXYGEN__)

void sysclk_enable_usb(void) {
    Assert(CONFIG_USBCLK_DIV > 0);
#ifdef CONFIG_PLL0_SOURCE
    if (CONFIG_USBCLK_SOURCE == USBCLK_SRC_PLL0) {
        struct pll_config pllcfg;
        pll_enable_source(CONFIG_PLL0_SOURCE);
        pll_config_defaults(&pllcfg, 0);
        pll_enable(&pllcfg, 0);
        pll_wait_for_lock(0);
        pmc_switch_udpck_to_pllack(CONFIG_USBCLK_DIV - 1);
        pmc_enable_udpck();
        return;
    }
#endif
    if (CONFIG_USBCLK_SOURCE == USBCLK_SRC_UPLL) {
        pmc_enable_upll_clock();
        pmc_switch_udpck_to_upllck(CONFIG_USBCLK_DIV - 1);
        pmc_enable_udpck();
        return;
    }
}

void sysclk_disable_usb(void) {
    pmc_disable_udpck();
}

#endif // CONFIG_USBCLK_SOURCE

void sysclk_init(void) {
    struct pll_config pllcfg;
    /* Set flash wait state to max in case the below clock switching. */
    system_init_flash(CHIP_FREQ_CPU_MAX);
    /* Config system clock setting */
    if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_RC) {
        osc_enable(OSC_SLCK_32K_RC);
        osc_wait_ready(OSC_SLCK_32K_RC);
        pmc_mck_set_division(CONFIG_SYSCLK_DIV);
        pmc_switch_mck_to_sclk(CONFIG_SYSCLK_PRES);
    } else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_XTAL) {
        osc_enable(OSC_SLCK_32K_XTAL);
        osc_wait_ready(OSC_SLCK_32K_XTAL);
        pmc_mck_set_division(CONFIG_SYSCLK_DIV);
        pmc_switch_mck_to_sclk(CONFIG_SYSCLK_PRES);
    } else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_BYPASS) {
        osc_enable(OSC_SLCK_32K_BYPASS);
        osc_wait_ready(OSC_SLCK_32K_BYPASS);
        pmc_mck_set_division(CONFIG_SYSCLK_DIV);
        pmc_switch_mck_to_sclk(CONFIG_SYSCLK_PRES);
    } else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_4M_RC) {
        /* Already running from SYSCLK_SRC_MAINCK_4M_RC */
    } else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_8M_RC) {
        osc_enable(OSC_MAINCK_8M_RC);
        osc_wait_ready(OSC_MAINCK_8M_RC);
        pmc_mck_set_division(CONFIG_SYSCLK_DIV);
        pmc_switch_mck_to_mainck(CONFIG_SYSCLK_PRES);
    } else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_12M_RC) {
        osc_enable(OSC_MAINCK_12M_RC);
        osc_wait_ready(OSC_MAINCK_12M_RC);
        pmc_mck_set_division(CONFIG_SYSCLK_DIV);
        pmc_switch_mck_to_mainck(CONFIG_SYSCLK_PRES);
    } else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_XTAL) {
        osc_enable(OSC_MAINCK_XTAL);
        osc_wait_ready(OSC_MAINCK_XTAL);
        pmc_mck_set_division(CONFIG_SYSCLK_DIV);
        pmc_switch_mck_to_mainck(CONFIG_SYSCLK_PRES);
    } else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_BYPASS) {
        osc_enable(OSC_MAINCK_BYPASS);
        osc_wait_ready(OSC_MAINCK_BYPASS);
        pmc_mck_set_division(CONFIG_SYSCLK_DIV);
        pmc_switch_mck_to_mainck(CONFIG_SYSCLK_PRES);
    }
#ifdef CONFIG_PLL0_SOURCE
    else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_PLLACK) {
        pll_enable_source(CONFIG_PLL0_SOURCE);
        pll_config_defaults(&pllcfg, 0);
        pll_enable(&pllcfg, 0);
        pll_wait_for_lock(0);
        pmc_mck_set_division(CONFIG_SYSCLK_DIV);
        pmc_switch_mck_to_pllack(CONFIG_SYSCLK_PRES);
    }
#endif
    else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_UPLLCK) {
        pll_enable_source(CONFIG_PLL1_SOURCE);
        pll_config_defaults(&pllcfg, 1);
        pll_enable(&pllcfg, 1);
        pll_wait_for_lock(1);
        pmc_mck_set_division(CONFIG_SYSCLK_DIV);
        pmc_switch_mck_to_upllck(CONFIG_SYSCLK_PRES);
    }
    /* Update the SystemFrequency variable */
    SystemCoreClockUpdate();
    /* Set a flash wait state depending on the new cpu frequency */
    system_init_flash(sysclk_get_cpu_hz());
#if (defined CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)
    /* Signal that the internal frequencies are setup */
    sysclk_initialized = 1;
#endif
}
