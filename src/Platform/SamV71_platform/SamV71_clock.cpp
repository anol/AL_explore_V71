#include "SamV71_clock.h"
#include "sam.h"

namespace SamV71
{
    constexpr uint32_t Slow_clock_frequency{32'768};
    constexpr uint32_t Frequency_measure_slow_clock_periods{16};
    constexpr uint32_t RC_oscillator_frequency{12'000'000};
    constexpr uint32_t XTAL_oscillator_frequency{12'000'000};
    constexpr uint32_t XTAL_oscillator_startup_time{64};
    constexpr uint32_t PLLA_front_end_divider{1};
    constexpr uint32_t PLLA_input_multiplier{25};
    constexpr uint32_t PLLA_frequency{XTAL_oscillator_frequency * PLLA_input_multiplier / PLLA_front_end_divider};
    constexpr uint32_t Master_clock_frequency{PLLA_frequency / 2};
    static_assert(Master_clock_frequency == 150'000'000);

    volatile static uint32_t the_cycle_count{0u};
    volatile static uint32_t the_measured_frequency{0u};
}

extern "C" {
uint32_t SystemCoreClock{SamV71::RC_oscillator_frequency};
}

namespace SamV71
{
    void SamV71_clock::initialize()
    {
        disable_watchdog();
        // initialize_main_clock();
        initialize_PLLA();
        initialize_master_clock();
        SystemCoreClock = SamV71::Master_clock_frequency;
        // Enable Peripheral Clock
        PMC_REGS->PMC_PCER0 = 0x35c00U;
    }

    void SamV71_clock::initialize_main_clock()
    {
        // Enable Main Crystal Oscillator and define the startup time
        PMC_REGS->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTEN_Msk
            | CKGR_MOR_MOSCXTST(XTAL_oscillator_startup_time);
        while (!(PMC_REGS->PMC_SR & PMC_SR_MOSCXTS_Msk))
        {
            __NOP();
        }
        // Switch Main Clock to Main Crystal Oscillator clock
        PMC_REGS->CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL_Msk;
        while (!(PMC_REGS->PMC_SR & PMC_SR_MOSCSELS_Msk))
        {
            __NOP();
        }
        // Check Main CLock frequency
        while (!(PMC_REGS->CKGR_MCFR & CKGR_MCFR_MAINFRDY_Msk))
        {
            __NOP();
        }
        the_cycle_count = PMC_REGS->CKGR_MCFR & CKGR_MCFR_MAINF_Msk;
        the_measured_frequency = (the_cycle_count * Slow_clock_frequency) / Frequency_measure_slow_clock_periods;
    }

    void SamV71_clock::initialize_PLLA()
    {
        PMC_REGS->CKGR_PLLAR = CKGR_PLLAR_ONE_Msk | CKGR_PLLAR_MULA(PLLA_input_multiplier -1) |
            CKGR_PLLAR_DIVA(PLLA_front_end_divider) | CKGR_PLLAR_PLLACOUNT(0x3Fu);
        while (!(PMC_REGS->PMC_SR & PMC_SR_LOCKA_Msk))
        {
            __NOP();
        }
    }

    void SamV71_clock::initialize_master_clock()
    {
        // Increasing flash wait states before switching master clock to PLLA
        EFC_REGS->EEFC_FMR = EEFC_FMR_CLOE_Msk | EEFC_FMR_FWS(6);
        while (!(EFC_REGS->EEFC_FSR & EEFC_FSR_FRDY_Msk))
        {
            __NOP();
        }
        // Select the master clock prescaler divider -> Processor Clock (HCLK)
        PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_PRES_Msk) | PMC_MCKR_PRES_CLK_1;
        while (!(PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk))
        {
            __NOP();
        }
        // Select the master clock output divider -> Host CLock (MCK) and Peripheral Clock Controller (PMC_PCR)
        PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_MDIV_Msk) | PMC_MCKR_MDIV_PCK_DIV2;
        while (!(PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk))
        {
            __NOP();
        }
        // Select clock source for the master clock
        PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_PLLA_CLK;
        while (!(PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk))
        {
            __NOP();
        }
    }

    void SamV71_clock::disable_watchdog()
    {
        WDT_REGS->WDT_MR = WDT_MR_WDDIS_Msk; // Disable the watchdog
        RSWDT_REGS->RSWDT_MR = RSWDT_MR_WDDIS_Msk; // Disable RSWDT
    }

    uint32_t SamV71_clock::get_frequency()
    {
        return Master_clock_frequency;
    }

    void SamV71_clock::enable_peripheral_clock(const uint32_t peripheral_id)
    {
        if (peripheral_id < 32)
        {
            PMC_REGS->PMC_PCER0 = 1 << peripheral_id;
        }
        else
        {
            PMC_REGS->PMC_PCER1 = 1 << (peripheral_id - 32);
        }
    }
}
