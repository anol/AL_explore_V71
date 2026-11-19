//
// Created by aeols on 2026-11-19.
//

module;

#include "FreeRTOS.h"
#include "portmacro.h"
#include "samv71q21b.h"
#include "component/twihs.h"

module Platform.SamV71_I2C:Handler;
import :Define;
import :Driver;
import Type.I2C_transfer_request;

namespace SamV71 {
    class SamV71_I2C_handler {
    public:
        enum { Number_of_I2C_peripherals = 1 };

        static SamV71_I2C_handler *optional_handler[Number_of_I2C_peripherals];

    private:
        static constexpr uint32_t Error_mask = TWIHS_SR_NACK_Msk | TWIHS_SR_ARBLST_Msk;

        const SamV71_I2C_define *optional_define;
        SamV71_I2C_driver &use_driver;
        Generic::I2C_transfer_request *optional_request{};
        bool the_stop_sent{};

    public:
        explicit SamV71_I2C_handler(const SamV71_I2C_define *define, SamV71_I2C_driver &driver)
            : optional_define(define), use_driver(driver) {
            if (define && define->the_ID_number < Number_of_I2C_peripherals) {
                optional_handler[define->the_ID_number] = this;
            }
        }

        void ISR();

    private:
        [[nodiscard]] bool is_busy() const { return optional_request != nullptr; }

        [[nodiscard]] bool is_ready() const { return optional_request == nullptr; }

        [[nodiscard]] bool is_frame_done() const;

        void ISR_pending_transaction(twihs_registers_t *base, uint32_t &context_switch);

        void ISR_transfer_complete(bool success, uint32_t &context_switch);

        void ISR_RX_ready(twihs_registers_t *base);

        void ISR_TX_ready(twihs_registers_t *base);

        void start_transaction(twihs_registers_t *base);
    };

    SamV71_I2C_handler *SamV71_I2C_handler::optional_handler[Number_of_I2C_peripherals]{};

    /*
     * One request is one START ... STOP frame, driven byte by byte from the ISR:
     *  - host read:  RXRDY drives the frame; STOP is requested once only the last byte is left
     *                (together with START for a single-byte read).
     *  - host write: TXRDY drives the frame; STOP is requested when the last byte has left THR.
     * The frame is complete on TXCOMP (STOP sent); NACK / ARBLST abort it with failure.
     * TXCOMP is also used as the one-shot "kick" to start an idle peripheral.
     */
    void SamV71_I2C_handler::ISR() {
        if (optional_define) {
            uint32_t context_switch{};
            auto *base = optional_define->the_base;
            const uint32_t status = base->TWIHS_SR; // read clears NACK / ARBLST / OVRE / UNRE
            const uint32_t active = status & base->TWIHS_IMR;
            if (is_busy()) {
                if (status & Error_mask) {
                    ISR_transfer_complete(false, context_switch);
                } else {
                    if (active & TWIHS_SR_RXRDY_Msk) {
                        ISR_RX_ready(base);
                    }
                    if (active & TWIHS_SR_TXRDY_Msk) {
                        ISR_TX_ready(base);
                    }
                    if ((active & TWIHS_SR_TXCOMP_Msk) && is_frame_done()) {
                        ISR_transfer_complete(true, context_switch);
                    }
                }
            }
            if (is_ready()) {
                if (base->TWIHS_SR & TWIHS_SR_TXCOMP_Msk) {
                    ISR_pending_transaction(base, context_switch);
                } else {
                    base->TWIHS_IDR = TWIHS_IDR_RXRDY(1) | TWIHS_IDR_TXRDY(1);
                    base->TWIHS_IER = TWIHS_IER_TXCOMP(1); // bus still busy (e.g. after NACK): wait for STOP
                }
            }
            portYIELD_FROM_ISR(context_switch);
        }
    }

    /*
     * Guards against a stale TXCOMP right after a frame was started: the frame is only done once
     * every byte was moved and STOP was requested.
     */
    bool SamV71_I2C_handler::is_frame_done() const {
        return optional_request && !optional_request->is_more_to_transfer() && the_stop_sent;
    }

    void SamV71_I2C_handler::ISR_RX_ready(twihs_registers_t *base) {
        const uint8_t data = 0xFF & base->TWIHS_RHR;
        if (optional_request->is_more_to_transfer()) {
            optional_request->put_next_byte(data);
            optional_request->count_transferred();
            if (optional_request->get_remaining() == 1) {
                base->TWIHS_CR = TWIHS_CR_STOP(1); // after next-to-last byte
                the_stop_sent = true;
            }
        }
        if (!optional_request->is_more_to_transfer()) {
            base->TWIHS_IDR = TWIHS_IDR_RXRDY(1);
            base->TWIHS_IER = TWIHS_IER_TXCOMP(1);
        }
    }

    void SamV71_I2C_handler::ISR_TX_ready(twihs_registers_t *base) {
        if (optional_request->is_more_to_transfer()) {
            base->TWIHS_THR = TWIHS_THR_TXDATA(optional_request->get_next_byte());
            optional_request->count_transferred();
        } else {
            base->TWIHS_CR = TWIHS_CR_STOP(1);
            the_stop_sent = true;
            base->TWIHS_IDR = TWIHS_IDR_TXRDY(1);
            base->TWIHS_IER = TWIHS_IER_TXCOMP(1);
        }
    }

    void SamV71_I2C_handler::ISR_transfer_complete(const bool success, uint32_t &context_switch) {
        auto *request = optional_request;
        optional_request = nullptr;
        the_stop_sent = false;
        request->set_success(success);
        request->get_semaphore().ISR_give(context_switch);
    }

    void SamV71_I2C_handler::ISR_pending_transaction(twihs_registers_t *base, uint32_t &context_switch) {
        while (Abstract::Abstract_request *request = use_driver.ISR_get_request(context_switch)) {
            auto *i2c_request = dynamic_cast<Generic::I2C_transfer_request *>(request);
            if (i2c_request == nullptr) {
                continue; // not an I2C request: drop it rather than dereference null
            }
            if (!i2c_request->is_more_to_transfer()) {
                // zero-length transfer: not supported by the TWIHS, complete with failure
                i2c_request->set_success(false);
                i2c_request->get_semaphore().ISR_give(context_switch);
                continue;
            }
            optional_request = i2c_request;
            start_transaction(base);
            return;
        }
        use_driver.disable_I2C();
    }

    void SamV71_I2C_handler::start_transaction(twihs_registers_t *base) {
        const bool is_read = optional_request->is_read();
        the_stop_sent = false;
        base->TWIHS_IDR = TWIHS_IDR_TXCOMP(1) | TWIHS_IDR_RXRDY(1) | TWIHS_IDR_TXRDY(1);
        base->TWIHS_MMR = TWIHS_MMR_DADR(optional_request->get_device_address()) |
                          TWIHS_MMR_MREAD(is_read ? 1 : 0) | TWIHS_MMR_IADRSZ(0);
        base->TWIHS_IER = TWIHS_IER_NACK(1) | TWIHS_IER_ARBLST(1);
        if (is_read) {
            if (base->TWIHS_SR & TWIHS_SR_RXRDY_Msk) {
                (void) base->TWIHS_RHR; // flush any stale byte
            }
            if (optional_request->get_remaining() == 1) {
                base->TWIHS_CR = TWIHS_CR_START(1) | TWIHS_CR_STOP(1);
                the_stop_sent = true;
            } else {
                base->TWIHS_CR = TWIHS_CR_START(1);
            }
            base->TWIHS_IER = TWIHS_IER_RXRDY(1);
        } else {
            // In host write mode, writing THR sends START + address + first byte.
            base->TWIHS_THR = TWIHS_THR_TXDATA(optional_request->get_next_byte());
            optional_request->count_transferred();
            base->TWIHS_IER = TWIHS_IER_TXRDY(1);
        }
    }
} // SamV71

extern "C" {
void ISR_TWIHS0() {
    if (auto *handler = SamV71::SamV71_I2C_handler::optional_handler[0]) {
        handler->ISR();
    } else {
        NVIC_DisableIRQ(IRQn::TWIHS0_IRQn);
        NVIC_ClearPendingIRQ(TWIHS0_IRQn);
    }
}
}
