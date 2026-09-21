#include <cstdio>

#include "SamV71_IO_pin.h"
#include "V71_EK_pin_table.h"
#include "component/matrix.h"

namespace SamV71
{
    using Pin = Abstract::Abstract_IO_pin;

    void V71_EK_pin_table::initialize()
    {
        initialize_matrix();
        SamV71_IO_pin::initialize_clocks();
        set_phase(Abstract::Abstract_IO_pin::Start_up);
    }

    void V71_EK_pin_table::initialize_matrix()
    {
        // Release the JTAG TDI pin
        MATRIX_REGS->CCFG_SYSIO |= CCFG_SYSIO_SYSIO4_Msk;
        // Release the JTAG ERASE pin
        MATRIX_REGS->CCFG_SYSIO |= CCFG_SYSIO_SYSIO12_Msk;
    }

    Status_code V71_EK_pin_table::set_phase(Pin::Pin_phase phase)
    {
        uint8_t id_counter{};
        for (auto& pin : the_pin_table)
        {
            if (pin.initialize(id_counter++, phase).failed())
            {
                return Status_code::Failure();
            }
        }
        return Status_code::Success();
    }

    Status_code V71_EK_pin_table::for_each_pin(void* user, void (*func)(void*, Abstract::Abstract_IO_pin&))
    {
        for (auto& pin : the_pin_table)
        {
            if (func) { func(user, pin); }
        }
        return Status_code::Success();
    }

    Pin& V71_EK_pin_table::get_pin(const Abstract::Abstract_pin_id id)
    {
        if (id < Domain::Number_of_pins)
        {
            return the_pin_table[id];
        }
        return the_pin_table[Domain::Pin_not_used];
    }

    void V71_EK_pin_table::print_diagnostics() const
    {
        printf("Pin manager\r\n");
        for (auto& pin : the_pin_table)
        {
            if (pin.is_used())
            {
                pin.print_diagnostics();
            }
        }
    }
}
