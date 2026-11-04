//
// Created by aeols on 2026-11-04.
//

module;

#include "FreeRTOS.h"
#include "portmacro.h"
#include "samv71q21b.h"
#include "component/spi.h"

module Platform.SamV71_SPI:Handler;
import :Define;
import :Driver;
import Type.Transfer_request;

namespace SamV71
{
    class SamV71_SPI_handler
    {
    public:
        enum { Number_of_SPI_peripherals = 1 };

        static SamV71_SPI_handler* optional_handler[Number_of_SPI_peripherals];

    private:
        const SamV71_SPI_define* optional_define;
        SamV71_SPI_driver& use_driver;
        Generic::SPI_transfer_request* optional_request{};

    public:
        explicit SamV71_SPI_handler(const SamV71_SPI_define* define, SamV71_SPI_driver& driver)
            : optional_define(define), use_driver(driver)
        {
            if (define->the_ID_number < Number_of_SPI_peripherals)
            {
                optional_handler[define->the_ID_number] = this;
            }
        }

        void ISR();

    private:
        [[nodiscard]] bool is_ready() const { return optional_request == nullptr; }

        void ISR_pending_transaction(uint32_t& context_switch);

        void ISR_check_progress(uint32_t& context_switch);

        void ISR_transfer_complete(uint32_t& context_switch);

        void ISR_RX_ready() const;

        void ISR_TX_ready() const;
    };

    SamV71_SPI_handler* SamV71_SPI_handler::optional_handler[Number_of_SPI_peripherals]{};

    void SamV71_SPI_handler::ISR()
    {
        if (optional_define)
        {
            uint32_t context_switch{};
            auto* base = optional_define->the_base;
            const uint32_t interrupt_status = base->SPI_SR;
            if (interrupt_status & SPI_SR_TDRE_Msk)
            {
                ISR_TX_ready();
            }
            if (interrupt_status & SPI_SR_RDRF_Msk)
            {
                ISR_RX_ready();
            }
            ISR_check_progress(context_switch);
            portYIELD_FROM_ISR(context_switch);
        }
    }

    void SamV71_SPI_handler::ISR_RX_ready() const
    {
        if (optional_define)
        {
            auto* base = optional_define->the_base;
            const uint8_t data = 0xFF & base->SPI_RDR;
            if (optional_request && optional_request->is_more_to_receive())
            {
                if (auto* buffer = optional_request->get_receive_buffer())
                {
                    const auto count = optional_request->get_receive_count();
                    *(buffer + count) = data;
                    optional_request->count_received();
                }
            }
            else
            {
                base->SPI_IDR = SPI_IDR_RDRF(1);
            }
        }
    }

    void SamV71_SPI_handler::ISR_TX_ready() const
    {
        if (optional_define)
        {
            auto* base = optional_define->the_base;
            if (optional_request && optional_request->is_more_to_send())
            {
                if (const auto* buffer = optional_request->get_transmit_buffer())
                {
                    const auto count = optional_request->get_send_count();
                    const uint8_t data = *(buffer + count);
                    base->SPI_TDR = SPI_TDR_TD(data);
                    optional_request->count_sent();
                }
                else
                {
                    base->SPI_IDR = SPI_IDR_TDRE(1);
                }
            }
            else
            {
                base->SPI_IDR = SPI_IDR_TDRE(1);
            }
        }
    }

    void SamV71_SPI_handler::ISR_check_progress(uint32_t& context_switch)
    {
        if (optional_request && !optional_request->is_more_to_receive())
        {
            ISR_transfer_complete(context_switch);
            ISR_pending_transaction(context_switch);
        }
    }

    void SamV71_SPI_handler::ISR_transfer_complete(uint32_t& context_switch)
    {
        if (optional_request)
        {
            const auto* request = optional_request;
            optional_request = nullptr;
            request->get_chip_select().set();
            request->get_semaphore().ISR_give(context_switch);
        }
        use_driver.disable_SPI();
    }

    void SamV71_SPI_handler::ISR_pending_transaction(uint32_t& context_switch)
    {
        if (is_ready())
        {
            if (Abstract::Abstract_request* request = use_driver.ISR_receive(context_switch))
            {
                optional_request = static_cast<Generic::SPI_transfer_request*>(request);
                optional_request->get_chip_select().clear();
                use_driver.enable_SPI();
            }
        }
    }

} // SamV71

extern "C" {
void ISR_SPI0()
{
    if (auto* handler = SamV71::SamV71_SPI_handler::optional_handler[0])
    {
        handler->ISR();
    }
    else
    {
        NVIC_DisableIRQ(IRQn::SPI0_IRQn);
        NVIC_ClearPendingIRQ(SPI0_IRQn);
    }
}
}
