#include <stdio.h>

#include "SamV71_IO_pin.h"
#include "core_cm7.h"
#include "samv71q21b.h"
#include "SamV71_clock.h"

namespace SamV71
{
    using If = Abstract::Abstract_IO_pin;

    static pio_registers_t* const pio_of_port[] = {PIOA_REGS, PIOB_REGS, PIOC_REGS, PIOD_REGS, PIOE_REGS};

    SamV71_IO_pin::SamV71_IO_pin(uint8_t id, const char* name, Pin_phase phase, Pin_port port, uint8_t pin,
                                 Pin_mode mux, Pin_type type, uint8_t strength, bool default_state) :
        the_name(name),
        optional_base(pio_of_port[port]),
        the_pin_mask(1U << pin),
        the_id(id),
        the_phase(phase),
        the_port(port),
        the_pin(pin),
        the_mode(mux),
        the_type(type),
        the_strength(strength),
        the_default_state(default_state)
    {
    }

    Error_codes::Error_code SamV71_IO_pin::initialize(const uint8_t id, const Pin_phase phase)
    {
        if (optional_base != nullptr)
        {
            if (id != the_id)
            {
                // Assert failed: the pin id does not match
                return {0xF1};
            }
            if (!the_pin_mask)
            {
                // Assert failed: the pin mask is zero
                return {0xF2};
            }
            if (phase >= the_phase)
            {
                if (is_GPIO())
                {
                    if (the_default_state)
                    {
                        set();
                    }
                    else
                    {
                        clear();
                    }
                }
                pin_configure(the_port, the_pin, the_mode, the_type);
            }
        }
        return {};
    }

    bool SamV71_IO_pin::get() const
    {
        if (optional_base != nullptr)
        {
            return ((optional_base->PIO_PDSR) & the_pin_mask) != 0;
        }
        else
        {
            return {};
        }
    }

    void SamV71_IO_pin::set()
    {
        if (optional_base != nullptr)
        {
            optional_base->PIO_SODR = the_pin_mask;
            __DSB();
        }
    }

    void SamV71_IO_pin::clear()
    {
        if (optional_base != nullptr)
        {
            optional_base->PIO_CODR = the_pin_mask;
            __DSB();
        }
    }

    void SamV71_IO_pin::toggle()
    {
        if (optional_base != nullptr)
        {
            if (((optional_base->PIO_PDSR) & the_pin_mask) == 0)
            {
                optional_base->PIO_SODR = the_pin_mask;
                __DSB();
            }
            else
            {
                optional_base->PIO_CODR = the_pin_mask;
                __DSB();
            }
        }
    }

    void SamV71_IO_pin::pulse()
    {
        if (optional_base != nullptr)
        {
            optional_base->PIO_SODR = the_pin_mask;
            __DSB();
            optional_base->PIO_CODR = the_pin_mask;
            __DSB();
        }
    }

    void SamV71_IO_pin::pulse(uint32_t count)
    {
        if (optional_base != nullptr)
        {
            for (; count > 0; count--)
            {
                optional_base->PIO_SODR = the_pin_mask;
                __DSB();
                optional_base->PIO_CODR = the_pin_mask;
                __DSB();
            }
        }
    }


    void SamV71_IO_pin::pin_configure(Pin_port port, uint8_t pin, Pin_mode mode, Pin_type type)
    {
        auto* const pio = pio_of_port[port];
        const uint32_t mask = 1u << pin;

        /* Peripheral mux: claim pin for GPIO, or route it to peripheral A/B/C/D */
        if (mode == Mode_GPIO)
        {
            pio->PIO_PER = mask;
        }
        else
        {
            uint32_t abcdsr0 = pio->PIO_ABCDSR[0];
            uint32_t abcdsr1 = pio->PIO_ABCDSR[1];
            if (mode == Mode_B || mode == Mode_D) abcdsr0 |= mask;
            else abcdsr0 &= ~mask;
            if (mode == Mode_C || mode == Mode_D) abcdsr1 |= mask;
            else abcdsr1 &= ~mask;
            pio->PIO_ABCDSR[0] = abcdsr0;
            pio->PIO_ABCDSR[1] = abcdsr1;
            pio->PIO_PDR = mask;
        }

        /* Direction (only meaningful while PIO itself owns the pin) */
        if (mode == Mode_GPIO)
        {
            switch (type)
            {
            case Input_normal:
            case Input_pull_up:
            case Input_pull_down:
            case Input_Schmitt_trigger:
                pio->PIO_ODR = mask;
                break;
            case Out_normal:
            case Out_open_drain:
            case Out_open_drain_pull_up:
                pio->PIO_OER = mask;
                break;
            }
        }

        /* Pull-up / pull-down — mutually exclusive on this controller */
        switch (type)
        {
        case Input_pull_up:
        case Out_open_drain_pull_up:
            pio->PIO_PPDDR = mask;
            pio->PIO_PUER = mask;
            break;
        case Input_pull_down:
            pio->PIO_PUDR = mask;
            pio->PIO_PPDER = mask;
            break;
        default:
            pio->PIO_PUDR = mask;
            pio->PIO_PPDDR = mask;
            break;
        }

        /* Multi-drive / open-drain */
        switch (type)
        {
        case Out_open_drain:
        case Out_open_drain_pull_up:
            pio->PIO_MDER = mask;
            break;
        default:
            pio->PIO_MDDR = mask;
            break;
        }

        /* Schmitt trigger — register bit is inverted: 1 = disabled, 0 = enabled */
        if (type == Input_Schmitt_trigger) pio->PIO_SCHMITT &= ~mask;
        else pio->PIO_SCHMITT |= mask;
    }

    void SamV71_IO_pin::initialize_clocks()
    {
        Abstract_clock::enable_peripheral_clock(ID_PIOA);
        Abstract_clock::enable_peripheral_clock(ID_PIOB);
        Abstract_clock::enable_peripheral_clock(ID_PIOC);
        Abstract_clock::enable_peripheral_clock(ID_PIOD);
        Abstract_clock::enable_peripheral_clock(ID_PIOE);
    }
} // SamV71
