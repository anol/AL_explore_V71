/*
* Copyright (C) 2026 Integrated Detector Electronics AS
* All Rights Reserved.
*
* NOTICE: All information contained herein is, and remains
* the property of Integrated Detector Electronics AS and its suppliers,
* if any. The intellectual and technical concepts contained
* herein are proprietary to Integrated Detector Electronics AS
* and its suppliers and may be covered by Norwegian, EU. or U.S. patents,
* patents in process, and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Integrated Detector Electronics AS.
*/

/**
* @file   SamV71_clock.cpp
* @author AndersEmilOlsen, IDEAS
* @date   25.08.2026
* @brief  
*/

#include "SamV71_clock.h"
#include "sam.h"

namespace SamV71 {
    constexpr uint32_t the_clock_frequency{150'000'000};

    void SamV71_clock::initialize() {
        initialize_main_clock();
        initialize_PLLA();
        initialize_master_clock();
    }

    void SamV71_clock::initialize_main_clock() {
        // Enable Main Crystal Oscillator
        PMC_REGS->CKGR_MOR = CKGR_MOR_KEY_PASSWD | (PMC_REGS->CKGR_MOR & ~CKGR_MOR_MOSCXTBY_Msk) | CKGR_MOR_MOSCXTEN_Msk | CKGR_MOR_MOSCXTST(CHIP_FREQ_SLCK_RC);
        while (!(PMC_REGS->PMC_SR & PMC_SR_MOSCXTS_Msk)) {
            __NOP();
        }
        // Switch Main Clock to Main Crystal Oscillator clock
        PMC_REGS->CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL_Msk;
        // Switch Master Clock to Main Clock
        PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_MAIN_CLK;
        while (!(PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk)) {
            __NOP();
        }
        PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | PMC_MCKR_PRES_CLK_1;
        while (!(PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk)) {
            __NOP();
        }
    }

    void SamV71_clock::initialize_PLLA() {
        uint32_t input_multiplier  = 15u; // 25 - 1
        uint32_t front_end_divider = 3u;  // 1
        PMC_REGS->CKGR_PLLAR       = CKGR_PLLAR_ONE_Msk | CKGR_PLLAR_MULA(0u);
        PMC_REGS->CKGR_PLLAR       = CKGR_PLLAR_ONE_Msk | CKGR_PLLAR_MULA(input_multiplier) | CKGR_PLLAR_DIVA(front_end_divider) | CKGR_PLLAR_PLLACOUNT(0x3Fu);
        while ((PMC_REGS->PMC_SR & PMC_SR_LOCKA_Msk) != PMC_SR_LOCKA_Msk) {
            __NOP();
        }
    }

    void SamV71_clock::initialize_master_clock() {
        // Program PMC_MCKR.PRES and wait for PMC_SR.MCKRDY to be set
        PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_PRES_Msk) | PMC_MCKR_PRES_CLK_1;
        while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk) {
            __NOP();
        }
        // Program PMC_MCKR.MDIV and Wait for PMC_SR.MCKRDY to be set
        PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_MDIV_Msk) | PMC_MCKR_MDIV_PCK_DIV2;
        while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk) {
            __NOP();
        }
        // Program PMC_MCKR.CSS and Wait for PMC_SR.MCKRDY to be set
        PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_PLLA_CLK;
        while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk) {
            __NOP();
        }
    }

    uint32_t SamV71_clock::get_frequency() {
        return the_clock_frequency;
    }

    void SamV71_clock::enable_peripheral_clock(const uint32_t peripheral_id) {
        if (peripheral_id < 32) {
            PMC_REGS->PMC_PCER0 = 1 << peripheral_id;
        } else {
            PMC_REGS->PMC_PCER1 = 1 << (peripheral_id - 32);
        }
    }
}
