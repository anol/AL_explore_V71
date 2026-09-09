/*
* Copyright (C) 2020-2026 Integrated Detector Electronics AS
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
* @file   U575xG_embedded_flash.cpp
* @author AndersEmilOlsen, IDEAS
* @date   04.02.2026
* @brief  
*/

#include "U575xG_embedded_flash.h"

#include <cstring>

#include <stdio.h>
#include "stm32u575xx.h"
#include "stm32u5xx_hal.h"

extern uint32_t ld_repository_addr;
extern uint32_t ld_repository_size;

namespace STM32U575RG {
    static uint32_t cnt_wait_flags{};
    static uint32_t cnt_alignment_error{};
    static uint32_t cnt_clear_errors{};
    static uint32_t cnt_param_error{};
    static uint32_t cnt_end_of_operation{};
    static uint32_t cnt_erase{};
    static uint32_t cnt_old_error{};
    static uint32_t cnt_operation_error{};
    static uint32_t cnt_page_erase_error{};
    static uint32_t cnt_quad_ok{};
    static uint32_t cnt_quad_failed{};
    static uint32_t cnt_programming_error{};
    static uint32_t cnt_sequence_error{};
    static uint32_t cnt_size_error{};
    static uint32_t cnt_status_error{};
    static uint32_t cnt_timeout_error{};
    static uint32_t cnt_unlock_error{};
    static uint32_t cnt_wait_ceased_failed{};
    static uint32_t cnt_wait_ceased{};
    static uint32_t cnt_wait_CR_failed{};
    static uint32_t cnt_wait_CR{};
    static uint32_t cnt_wait_raised_failed{};
    static uint32_t cnt_wait_raised{};
    static uint32_t cnt_write_option_error{};
    static uint32_t cnt_write_protection_error{};

    enum Control_flags: uint32_t {
        Control_flags_to_clear = FLASH_NSCR_PER | FLASH_NSCR_PG | FLASH_NSCR_MER1 | FLASH_NSCR_MER2 |
                                 FLASH_NSCR_PNB | FLASH_NSCR_BWR | FLASH_NSCR_BKER |
                                 FLASH_NSCR_STRT | FLASH_NSCR_OPTSTRT,
        Control_flags_for_erase = FLASH_NSCR_PER | FLASH_NSCR_STRT,
        Control_flags_for_program = FLASH_NSCR_PG,
        Status_flags_to_clear = FLASH_FLAG_OPERR | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR |
                                FLASH_FLAG_SIZERR | FLASH_FLAG_PGSERR | FLASH_FLAG_OPTWERR,
    };

    static auto const Left_page_address{reinterpret_cast<uint32_t *>(U575xG_embedded_flash::Left_page_base)};
    static auto const Right_page_address{reinterpret_cast<uint32_t *>(U575xG_embedded_flash::Right_page_base)};

    bool U575xG_embedded_flash::is_write_open{};

    uint32_t *U575xG_embedded_flash::get_left_page() { return Left_page_address; }

    uint32_t *U575xG_embedded_flash::get_right_page() { return Right_page_address; }

    bool U575xG_embedded_flash::erase_left_page() { return erase_page(Flash_bank2, Left_page); }

    bool U575xG_embedded_flash::erase_right_page() { return erase_page(Flash_bank2, Right_page); }

    bool U575xG_embedded_flash::assert_address(const uint32_t *address) const {
        if (address < Left_page_address || address > reinterpret_cast<uint32_t *>(End_of_flash)) {
            printf("Illegal u-page address: 0x%08X\r\n", reinterpret_cast<uint32_t>(address));
            return false;
        }
        return true;
    }

    void U575xG_embedded_flash::read_page(uint32_t *buffer, const uint32_t *page, const uint32_t size_8) {
        memcpy(buffer, page, size_8);
    }

    bool U575xG_embedded_flash::read_quad(const uint32_t *address, uint32_t *quadword) {
        if (0xF & reinterpret_cast<uint32_t>(address)) {
            printf("Update: Illegal address 0x%08X\r\n", address);
            return false;
        }
        clear_error_status();
        clear_end_of_operation();
        auto success = busy_wait_for_ceased_flag(FLASH_FLAG_BSY);
        if (success) {
            *quadword++ = *address++;
            *quadword++ = *address++;
            *quadword++ = *address++;
            *quadword = *address;
        }
        return success;
    }

    void U575xG_embedded_flash::begin_programming() {
        wait_for_complete(Operation_timeout_1s);
        wait_for_timeout(Operation_timeout_1s);
    }

    void U575xG_embedded_flash::end_programming() {
        wait_for_complete(Operation_timeout_1s);
        wait_for_timeout(Operation_timeout_1s);
        // Bank 2 power-down mode request
        auto access_control_register = FLASH_NS->ACR;
        FLASH_NS->ACR = FLASH_ACR_PDREQ2 | access_control_register;
    }

    //
    // Erase a page
    // 1. Check that no flash memory operation is ongoing by checking BSY in FLASH_NSSR or FLASH_SECSR.
    // 2. Check and clear all error programming flags due to a previous programming. If not, PGSERR is set.
    // 3. Set PER bit and select the page to erase (PNB) with the associated bank (BKER) in FLASH_NSCR or FLASH_SECCR.
    // 4. Set STRT in FLASH_NSCR or FLASH_SECCR.
    // 5. Wait for BSY to be cleared in FLASH_NSSR or FLASH_SECSR.
    //

    bool U575xG_embedded_flash::erase_page(const uint32_t bank, const uint32_t page) {
        if (!unlock_control_register()) {
            return false;
        }
        if ((Flash_bank1 != bank) && (Flash_bank2 != bank)) {
            printf("U575xG_embedded_flash: illegal bank=%d\r\n", static_cast<int>(bank));
            return false;
        }
        if (Pages_per_bank <= page) {
            printf("U575xG_embedded_flash: illegal page=%d\r\n", static_cast<int>(page));
            return false;
        }
        /* Wait for last operation to be completed */
        auto success = wait_for_complete(Operation_timeout_1s);
        if (success) {
            volatile uint32_t *control_register = &(FLASH_NS->NSCR);
            uint32_t control_flags = *control_register;
            control_flags &= ~Control_flags_to_clear;
            control_flags |= Control_flags_for_erase;
            if (Flash_bank1 == bank) {
                control_flags &= ~(FLASH_NSCR_BKER);
            } else {
                control_flags |= FLASH_NSCR_BKER;
            }
            control_flags |= (page << FLASH_NSCR_PNB_Pos);
            *control_register = control_flags;
            success = wait_for_complete(Operation_timeout_1s);
            *control_register &= ~Control_flags_to_clear;
        }
        if (success) {
            cnt_erase++;
        } else {
            cnt_page_erase_error++;
        }
        return success;
    }

    // The flash memory is programmed 137 bits at a time (128-bit data + 9 bits ECC).
    // Programming in a previously programmed address is not allowed except if the data to write
    // is full zero, and any attempt sets PROGERR flag in FLASH_NSSR or FLASH_SECSR.
    // It is only possible to program quad-word (4 x 32-bit data).
    // • Any attempt to write byte or half-word sets SIZERR flag in FLASH_NSSR or FLASH_SECSR.
    // • Any attempt to write a quad-word that is not aligned with a quad-word address sets PGAERR flag.
    //
    // Flash programming
    // 1. Check that no flash main memory operation is ongoing by checking BSY in FLASH_NSSR or FLASH_SECSR.
    // 2. Check that the write buffer is empty by checking WDW in FLASH_NSSR or FLASH_SECSR.
    // 3. Check and clear all error programming flags due to a previous programming. If not, PGSERR is set.
    // 4. Set PG bit in FLASH_NSCR or FLASH_SECCR.
    // 5. Perform the data write operation at the desired flash memory address, or in the OTP area.
    // Only a quad-word can be programmed and OTP can be only programmed in  non-secure access:
    // – Write a first word in an address aligned on a quad-word address.
    //   WDW bits in FLASH_NSSR and FLASH_SECSR are set to indicate that more data can be written in the write buffer.
    // – Write the second, third and fourth word in the same quad-word.
    // 6. The BSY bit gets set. WDW is reset automatically.
    // 7. Wait until BSY is cleared in FLASH_NSSR or FLASH_SECSR. The software must make sure that BSY is set
    //    or WDW is cleared before waiting for BSY to get cleared.
    // 8. If the EOP flag is set in FLASH_NSSR or FLASH_SECSR (meaning that the programming operation has succeeded
    //    and the EOPIE bit is set), it must be cleared by software.
    // 9. Clear PG in FLASH_NSCR or FLASH_SECCR) if there is no more programming request.
    //
    // Note: When the flash memory interface received a good sequence (a quad-word), programming is automatically
    // launched and BSY bits are set. The internal oscillator HSI16 (16 MHz) is enabled automatically when PG bit is set,
    // and disabled automatically when PG bit is cleared, except if the HSI16 is previously enabled with HSION in RCC_CR.
    // No option bytes modification nor erase request is allowed when WDW bit is set.
    // Programming is possible only if the privileged and security attributes are respected (refer to Section 7.7).
    // If the user needs to program only one word, the quad-word must be completed with the erase value 0xFFFF FFFF
    // to launch automatically the programming. ECC is calculated from the quad-word to program.

    bool U575xG_embedded_flash::program_quad(uint32_t *address, const uint32_t quadword[4]) {
        if (0xF & reinterpret_cast<uint32_t>(address)) {
            cnt_param_error++;
            return false;
        }
        if (!unlock_control_register()) {
            return false;
        }
        auto success = wait_for_complete(Operation_timeout_10ms);
        if (success) {
            // Enter critical section: Disable interrupts
            uint32_t primask_bit = __get_PRIMASK();
            __disable_irq();
            volatile uint32_t *target = address;
            volatile uint32_t *control_register = &(FLASH_NS->NSCR);
            uint32_t control_flags = *control_register;
            control_flags &= ~Control_flags_to_clear;
            control_flags |= Control_flags_for_program;
            __DSB();
            __ISB();
            *control_register = control_flags;
            __DSB();
            __ISB();
            if (!busy_wait_for_control_flag(FLASH_NSCR_PG)) success = false;
            *target++ = *quadword++;
            if (!busy_wait_for_raised_flag(FLASH_FLAG_WDW)) success = false;
            *target++ = *quadword++;
            *target++ = *quadword++;
            *target = *quadword;
            __DSB();
            __ISB();
            if (!busy_wait_for_ceased_flag(FLASH_FLAG_BSY | FLASH_FLAG_WDW)) success = false;
            *control_register &= ~(Control_flags_for_program);
            clear_end_of_operation();
            clear_error_status();
            // Exit critical section: restore previous priority mask
            __set_PRIMASK(primask_bit);
            if (success) {
                cnt_quad_ok++;
            } else {
                cnt_quad_failed++;
            }
        } else {
            cnt_old_error++;
        }
        return success;
    }

    // Bit 3 PROGERR: Nonsecure programming error
    // This bit is set by hardware when a nonsecure quad-word address to be programmed
    // contains a value different from all 1 before programming, except if the data to write is all 0.
    // This bit is cleared by writing 1.
    //
    // PROGERR: secure/nonsecure programming error
    // It is set when the word to program is pointing to an address:
    // - not previously erased
    // - already fully programmed to 0
    // - already partially programmed (contains 0 and 1) and the new value to program is not full zero
    // - for OTP programming, when the address is already partially programmed (contains 0 and 1)
    //
    //  Bit 7 PGSERR: Nonsecure programming sequence error
    //  This bit is set by hardware when programming sequence is not correct. It is cleared
    //  by writing 1. Refer to Section 7.3.9 for full conditions of error flag setting.
    //
    //  PGSERR: programming sequence error
    //  PGSERR is set if one of the following conditions occurs during a erase or program operation:
    // - A data is written when PG is cleared.
    // - A program operation is requested during erase: PG is set while MER1, MER2, or PER is set.
    // - In the erase sequence, PG is set while STRT is already set.
    // - In the erase sequence, if STRT is set while MER1, MER2, and PER are cleared.
    // - If page and mass erase are requested at the same time, STRT and PER are set while MER1 or MER2 is set.
    // - If an operation is started while the write buffer is waiting for the next data, STRT or OPTSTRT is set while WDW is already set.
    // - If STRT and OPTSTRT are set at the same time.
    // - A nonsecure PGSERR is set if the nonsecure STRT bit is set by a secure access.
    // - A secure PGSERR is set if PROGERR, SIZERR, PGAERR, WRPERR or PGSERR is already set due to a previous programming error.
    // - A nonsecure PGSERR is set if PROGERR, SIZERR, PGAERR, WRPERR, PGSERR, or OPTWERR is already set due to a previous programming error.

    bool U575xG_embedded_flash::check_status_flags(const uint32_t status_flags) {
        const uint32_t error_flags = status_flags & FLASH_FLAG_SR_ERRORS;
        if (error_flags) {
            cnt_status_error++;
            if (error_flags & FLASH_FLAG_OPERR) {
                cnt_operation_error = cnt_operation_error + 1;
            }
            if (error_flags & FLASH_FLAG_PROGERR) {
                cnt_programming_error++;
            }
            if (error_flags & FLASH_FLAG_WRPERR) {
                cnt_write_protection_error++;
            }
            if (error_flags & FLASH_FLAG_PGAERR) {
                cnt_alignment_error++;
            }
            if (error_flags & FLASH_FLAG_SIZERR) {
                cnt_size_error++;
            }
            if (error_flags & FLASH_FLAG_PGSERR) {
                cnt_sequence_error++;
            }
            if (error_flags & FLASH_FLAG_OPTWERR) {
                cnt_write_option_error++;
            }
        }
        return 0 == error_flags;
    }

    bool U575xG_embedded_flash::busy_wait_for_raised_flag(const uint32_t flags) {
        int countdown{Countdown_for_raised};
        volatile uint32_t *status_register = &(FLASH_NS->NSSR);
        uint32_t status_flags = *status_register;
        __DSB();
        __ISB();
        while (0 == (status_flags & flags) && 0 < countdown--) {
            status_flags = *status_register;
            __DSB();
            __ISB();
            if ((FLASH_FLAG_PROGERR | FLASH_FLAG_PGSERR) & status_flags) {
                cnt_wait_flags++;
                break;
            }
        }
        const auto success = 0 != (status_flags & flags);
        if (success) {
            cnt_wait_raised++;
        } else {
            cnt_wait_raised_failed++;
        }
        return success;
    }

    bool U575xG_embedded_flash::busy_wait_for_ceased_flag(const uint32_t flags) {
        int countdown{Countdown_for_ceased};
        volatile uint32_t *status_register = &(FLASH_NS->NSSR);
        uint32_t status_flags = *status_register;
        __DSB();
        __ISB();
        while (0 != (status_flags & flags) && 0 < countdown--) {
            status_flags = *status_register;
            __DSB();
            __ISB();
        }
        const auto success = 0 == (status_flags & flags);
        if (success) {
            cnt_wait_ceased++;
        } else {
            cnt_wait_ceased_failed++;
        }
        return success;
    }

    bool U575xG_embedded_flash::busy_wait_for_control_flag(const uint32_t flags) {
        int countdown{Countdown_for_control};
        volatile uint32_t *control_register = &(FLASH_NS->NSCR);
        uint32_t control_flags = *control_register;
        __DSB();
        __ISB();
        while (0 == (control_flags & flags) && 0 < countdown--) {
            control_flags = *control_register;
            __DSB();
            __ISB();
        }
        const auto success = 0 != (control_flags & flags);
        if (success) {
            cnt_wait_CR++;
        } else {
            cnt_wait_CR_failed++;
        }
        return success;
    }

    void U575xG_embedded_flash::wait_for_timeout(const uint32_t timeout) {
        uint32_t wakeup = HAL_GetTick() + timeout;
        while (true) {
            if (HAL_GetTick() >= wakeup) {
                return;
            }
        }
    }

    // Wait for the FLASH operation to complete by polling on BUSY and WDW flags to be reset.
    // Even if the FLASH operation fails, the BUSY & WDW flags will be reset, and an error flag will be set.

    bool U575xG_embedded_flash::wait_for_complete(const uint32_t timeout) {
        uint32_t wakeup = HAL_GetTick() + timeout;
        volatile uint32_t *status_register = &(FLASH_NS->NSSR);
        __DSB();
        __ISB();
        uint32_t status_flags = *status_register;
        __DSB();
        __ISB();
        while ((status_flags & (FLASH_FLAG_BSY | FLASH_FLAG_WDW)) != 0U) {
            if (HAL_GetTick() >= wakeup) {
                cnt_timeout_error++;
                clear_error_status();
                clear_end_of_operation();
                return false;
            }
            __DSB();
            __ISB();
            status_flags = *status_register;
            __DSB();
            __ISB();
        }
        clear_error_status();
        clear_end_of_operation();
        return check_status_flags(status_flags);
    }

    // FLASH_NSCR = 0xC0000000
    //- the FLASH_NSCR register is locked
    //     Bit 31 LOCK: Nonsecure lock
    //     This bit is set only. When set, the FLASH_NSCR register is locked. It is cleared by hardware
    //     after detecting the unlock sequence in FLASH_NSKEYR register.
    //     In case of an unsuccessful unlock operation, this bit remains set until the next system reset.
    //- the user options in FLASH_NSCR are locked
    //     Bit 30 OPTLOCK: Option lock
    //     This bit is set only. When set, all bits concerning user options in FLASH_NSCR register are
    //     locked. This bit is cleared by hardware after detecting the unlock sequence. LOCK bit
    //     in FLASH_NSCR must be cleared before doing the unlock sequence for OPTLOCK bit.
    //     In case of an unsuccessful unlock operation, this bit remains set until the next reset.
    //     Bits 31:0 NSKEY[31:0]: Flash memory nonsecure key
    //
    // FLASH nonsecure key register (FLASH_NSKEYR)
    // The following values must be written consecutively to unlock the FLASH_NSCR register,
    // allowing the flash memory nonsecure programming/erasing operations:
    // KEY1: 0x4567 0123
    // KEY2: 0xCDEF 89AB

    bool U575xG_embedded_flash::unlock_control_register() {
        volatile uint32_t *control_register = &(FLASH_NS->NSCR);
        __DSB();
        __ISB();
        if (0 != (*control_register & FLASH_NSCR_LOCK)) {
            /* Authorize the FLASH Registers access */
            volatile uint32_t *key_register = &(FLASH_NS->NSKEYR);
            __DSB();
            __ISB();
            *key_register = Unlock_key_1;
            __DSB();
            __ISB();
            *key_register = Unlock_key_2;
            __DSB();
            __ISB();
        }
        /* verify Flash is unlocked */
        auto success = 0 == (*control_register & FLASH_NSCR_LOCK);
        if (!success) {
            cnt_unlock_error++;
        }
        return success;
    }

    void U575xG_embedded_flash::clear_error_status() {
        constexpr auto error_flags = FLASH_FLAG_SR_ERRORS;
        volatile uint32_t *status_register = &(FLASH_NS->NSSR);
        __DSB();
        __ISB();
        const auto status_flags = *status_register;
        check_status_flags(status_flags);
        if (error_flags & status_flags) {
            /* Clear error flags */
            *status_register = error_flags;
            __DSB();
            __ISB();
            cnt_clear_errors++;
        }
    }

    // EOP: Nonsecure end of operation
    // This bit is set by hardware when one or more flash memory nonsecure operation (program/erase)
    // has been completed successfully. This bit is set only if the nonsecure end of operation interrupts
    // are enabled (EOPIE = 1 in FLASH_NSCR). This bit is cleared by writing 1.

    void U575xG_embedded_flash::clear_end_of_operation() {
        volatile uint32_t *status_register = &(FLASH_NS->NSSR);
        const auto end_of_operation = *status_register & FLASH_FLAG_EOP;
        if (end_of_operation) {
            /* Clear end of operation flag */
            __DSB();
            __ISB();
            *status_register = end_of_operation;
            __DSB();
            __ISB();
            cnt_end_of_operation++;
        }
    }

    void U575xG_embedded_flash::print_diag() {
        printf("Flash: mode=%s\r\n", is_open_for_write() ? "write" : "read");
        printf("Count: program=%d, eop=%d, erase=%d, errors=%d, CR=%d, SRup=%d SRdown=%d\r\n",
               cnt_quad_ok, cnt_end_of_operation, cnt_erase, cnt_clear_errors,
               cnt_wait_CR, cnt_wait_raised, cnt_wait_ceased);
        printf("Error: program=%d, unlock=%d, timeout=%d, old=%d, status=%d, erase=%d, param=%d\r\n",
               cnt_quad_failed, cnt_unlock_error, cnt_timeout_error, cnt_old_error, cnt_status_error,
               cnt_page_erase_error, cnt_param_error);
        printf("Timeout: flags=%d, wCR=%d, SRup=%d, SRdown=%d\r\n",
               cnt_wait_flags, cnt_wait_CR_failed, cnt_wait_raised_failed, cnt_wait_ceased_failed);
        printf("Flags: OPERR=%d, PROGERR=%d, WRPERR=%d, PGAERR=%d, SIZERR=%d, PGSERR=%d, OPTWERR=%d\r\n",
               cnt_operation_error,
               cnt_programming_error,
               cnt_write_protection_error,
               cnt_alignment_error,
               cnt_size_error,
               cnt_sequence_error,
               cnt_write_option_error);
        printf("Registers: ACR=0x%08X, NSCR = 0x%08X, NSSR = 0x%08X\r\n",
               FLASH_NS->NSSR, FLASH_NS->ACR, FLASH_NS->NSCR);
        printf("... OPSR = 0x%08X, ECCR = 0x%08X, PRIVCFGR = 0x%08X\r\n",
               FLASH_NS->OPSR, FLASH_NS->ECCR, FLASH_NS->PRIVCFGR);
    }
} // STM32U575RG
