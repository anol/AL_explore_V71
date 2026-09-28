module;

#include <cstdint>
#include "sam.h"
#include "samv71q21b.h"
#include "component/spi.h"

#include "FreeRTOS.h"

module Platform.SamV71_SPI;
import Type.Abstract_SPI;
import Type.Transfer_request;
import Platform.FreeRTOS_queue;
import Platform.SamV71_clock;
import Platform.SamV71_IO_pin;

namespace SamV71_SPI_definition
{
    static SamV71::SamV71_SPI* optional_SPI0{};

    namespace
    {
        struct Definition
        {
            spi_registers_t* the_base;
            IRQn_Type the_IRQ_number;
            uint32_t the_IRQ_priority;
        };
    }

    static Definition Definition_SPI0{
        .the_base = SPI0_REGS,
        .the_IRQ_number = SPI0_IRQn,
        .the_IRQ_priority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY,
    };

    static void* get_definition(const uint8_t id) { return id == 0 ? static_cast<void*>(&Definition_SPI0) : nullptr; }
}

extern "C" {
void ISR_SPI0()
{
    if (auto* driver = SamV71_SPI_definition::optional_SPI0)
    {
        driver->ISR();
    }
    else
    {
        NVIC_DisableIRQ(SPI0_IRQn);
        NVIC_ClearPendingIRQ(SPI0_IRQn);
    }
}
}

namespace SamV71
{
    using namespace SamV71_SPI_definition;

    SamV71_SPI::SamV71_SPI(const uint8_t id, Abstract::Abstract_IO_pin& chip_select)
        : the_id(id), use_chip_select(chip_select), optional_definition(get_definition(id))
    {
        configASSERT(optional_definition != nullptr);
        if (the_id == 0)
        {
            optional_SPI0 = this;
        }
    }

    void SamV71_SPI::initialize()
    {
        SamV71_clock::enable_peripheral_clock(SPI0_INSTANCE_ID);
        disable_SPI();
        setup_SPI_registers(SamV71_clock::get_frequency() / SPI_bitrate);
        if (the_id == 0)
        {
            optional_SPI0 = this;
        }
        enable_SPI();
        set_ready();
    }

    void SamV71_SPI::ISR()
    {
        if (optional_definition)
        {
            uint32_t context_switch{};
            auto* base = static_cast<Definition*>(optional_definition)->the_base;
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

    void SamV71_SPI::ISR_RX_ready() const
    {
        if (optional_definition)
        {
            auto* base = static_cast<Definition*>(optional_definition)->the_base;
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

    void SamV71_SPI::ISR_TX_ready() const
    {
        if (optional_definition)
        {
            auto* base = static_cast<Definition*>(optional_definition)->the_base;
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

    void SamV71_SPI::ISR_check_progress(uint32_t& context_switch)
    {
        if (optional_request && !optional_request->is_more_to_receive())
        {
            ISR_transfer_complete(context_switch);
            ISR_pending_transaction(context_switch);
        }
    }

    void SamV71_SPI::ISR_transfer_complete(uint32_t& context_switch)
    {
        unselect_chip();
        disable_SPI();
        if (optional_request)
        {
            if (auto* semaphore = optional_request->get_semaphore())
            {
                semaphore->ISR_give(context_switch);
            }
        }
        set_ready();
    }

    void SamV71_SPI::pending_transaction()
    {
        if (is_ready())
        {
            Abstract::Abstract_request* request{};
            if (the_queue.receive(&request))
            {
                optional_request = static_cast<Generic::Transfer_request*>(request);
                enable_SPI();
                select_chip();
            }
        }
    }

    void SamV71_SPI::ISR_pending_transaction(uint32_t& context_switch)
    {
        if (is_ready())
        {
            Abstract::Abstract_request* request{};
            if (the_queue.ISR_receive(&request, context_switch))
            {
                optional_request = static_cast<Generic::Transfer_request*>(request);
                enable_SPI();
                select_chip();
            }
        }
    }

    void SamV71_SPI::setup_SPI_registers(const uint32_t bitrate_divider) const
    {
        if (optional_definition)
        {
            auto* base = static_cast<Definition*>(optional_definition)->the_base;
            base->SPI_CSR[0] =
                SPI_CSR_SCBR(bitrate_divider) |
                SPI_CSR_CPOL(1) | // Clock polarity
                SPI_CSR_NCPHA(1) | // Clock phase
                SPI_CSR_DLYBS(0) | // Delay before select
                SPI_CSR_DLYBCT(0); // Delay between consecutive transfers
            base->SPI_MR = SPI_MR_MSTR(1) | SPI_MR_MODFDIS(1);
        }
    }

    void SamV71_SPI::enable_SPI() const
    {
        if (optional_definition)
        {
            auto* base = static_cast<Definition*>(optional_definition)->the_base;
            const auto irq_number = static_cast<Definition*>(optional_definition)->the_IRQ_number;
            const auto priority = static_cast<Definition*>(optional_definition)->the_IRQ_priority;
            base->SPI_CR |= SPI_CR_SPIEN(1);
            NVIC_DisableIRQ(irq_number);
            NVIC_SetPriority(irq_number, priority);
            base->SPI_IER = SPI_IER_RDRF(1) | SPI_IER_TDRE(1);
            NVIC_ClearPendingIRQ(irq_number);
            NVIC_EnableIRQ(irq_number);
        }
    }

    void SamV71_SPI::disable_SPI() const
    {
        if (optional_definition)
        {
            auto* base = static_cast<Definition*>(optional_definition)->the_base;
            const auto irq_number = static_cast<Definition*>(optional_definition)->the_IRQ_number;
            NVIC_DisableIRQ(irq_number);
            base->SPI_IDR = (
                SPI_IDR_RDRF(1) |
                SPI_IDR_TDRE(1) |
                SPI_IDR_MODF(1) |
                SPI_IDR_OVRES(1) |
                SPI_IDR_NSSR(1) |
                SPI_IDR_TXEMPTY(1) |
                SPI_IDR_UNDES(1));
            NVIC_ClearPendingIRQ(irq_number);
            NVIC_EnableIRQ(irq_number);
        }
    }

    bool SamV71_SPI::transfer(Abstract::Abstract_request* request)
    {
        bool success{};
        if (request)
        {
            if (auto* semaphore = request->get_semaphore())
            {
                if (the_queue.send(request))
                {
                    pending_transaction();
                    semaphore->take();
                    success = true;
                }
            }
        }
        return success;
    }
} // SamV71
