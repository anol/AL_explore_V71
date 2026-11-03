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
* @file   SamV71_SPI_interrupt.cpp
* @author AndersEmilOlsen, IDEAS
* @date   03.11.2026
* @brief  
*/

module;

#include "FreeRTOS.h"
#include "portmacro.h"
#include "samv71q21b.h"

module Platform.SamV71_SPI_interrupt;

namespace SamV71_SPI_definition {
    static SamV71::SamV71_SPI_interrupt *optional_SPI0{};
}

extern "C" {
void ISR_SPI0() {
    if (auto *handler = SamV71_SPI_definition::optional_SPI0) {
        handler->ISR();
    } else {
        NVIC_DisableIRQ(IRQn::SPI0_IRQn);
        NVIC_ClearPendingIRQ(SPI0_IRQn);
    }
}
}

namespace SamV71 {
    SamV71_SPI_interrupt::SamV71_SPI_interrupt(SamV71_SPI &driver) : use_driver(driver) {
        if (driver.get_id() == 0) {
            ::SamV71_SPI_definition::optional_SPI0 = this;
        }
    }

    void SamV71_SPI_interrupt::ISR() {
        auto *optional_definition = use_driver.get_definition();
        if (optional_definition) {
            uint32_t context_switch{};
            auto *base = optional_definition->the_base;
            const uint32_t interrupt_status = base->SPI_SR;
            if (interrupt_status & SPI_SR_TDRE_Msk) {
                ISR_TX_ready();
            }
            if (interrupt_status & SPI_SR_RDRF_Msk) {
                ISR_RX_ready();
            }
            ISR_check_progress(context_switch);
            portYIELD_FROM_ISR(context_switch);
        }
    }

    void SamV71_SPI_interrupt::ISR_RX_ready() const {
        if (auto *optional_definition = use_driver.get_definition()) {
            auto *base = optional_definition->the_base;
            const uint8_t data = 0xFF & base->SPI_RDR;
            if (optional_request && optional_request->is_more_to_receive()) {
                if (auto *buffer = optional_request->get_receive_buffer()) {
                    const auto count = optional_request->get_receive_count();
                    *(buffer + count) = data;
                    optional_request->count_received();
                }
            } else {
                base->SPI_IDR = SPI_IDR_RDRF(1);
            }
        }
    }

    void SamV71_SPI_interrupt::ISR_TX_ready() const {
        if (auto *optional_definition = use_driver.get_definition()) {
            auto *base = optional_definition->the_base;
            if (optional_request && optional_request->is_more_to_send()) {
                if (const auto *buffer = optional_request->get_transmit_buffer()) {
                    const auto count = optional_request->get_send_count();
                    const uint8_t data = *(buffer + count);
                    base->SPI_TDR = SPI_TDR_TD(data);
                    optional_request->count_sent();
                } else {
                    base->SPI_IDR = SPI_IDR_TDRE(1);
                }
            } else {
                base->SPI_IDR = SPI_IDR_TDRE(1);
            }
        }
    }

    void SamV71_SPI_interrupt::ISR_check_progress(uint32_t &context_switch) {
        if (optional_request && !optional_request->is_more_to_receive()) {
            ISR_transfer_complete(context_switch);
            ISR_pending_transaction(context_switch);
        }
    }

    void SamV71_SPI_interrupt::ISR_transfer_complete(uint32_t &context_switch) {
        if (optional_request) {
            const auto *request = optional_request;
            optional_request = nullptr;
            request->get_chip_select().set();
            request->get_semaphore().ISR_give(context_switch);
        }
        use_driver.disable_SPI();
    }

    void SamV71_SPI_interrupt::ISR_pending_transaction(uint32_t &context_switch) {
        if (is_ready()) {
            if (Abstract::Abstract_request *request = use_driver.ISR_receive(context_switch)) {
                optional_request = static_cast<Generic::SPI_transfer_request *>(request);
                optional_request->get_chip_select().clear();
                use_driver.enable_SPI();
            }
        }
    }
} // SamV71
