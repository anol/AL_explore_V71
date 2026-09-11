#include <cstdio>

#include "../../Platform/SamV71_platform/SamV71_IO_pin.h"
#include "V71_EK_pin_table.h"
#include "V71_EK_pin_manager.h"
#include "component/matrix.h"

namespace SamV71
{
    using namespace Dictionary;
    using Pin = Abstract::Abstract_IO_pin;

    Status_code V71_EK_pin_manager::initialize()
    {
        initialize_matrix();
        return set_phase(Abstract::Abstract_IO_pin::Start_up);
    }

    void V71_EK_pin_manager::initialize_matrix()
    {
        // Release the JTAG TDI pin
        MATRIX_REGS->CCFG_SYSIO |= CCFG_SYSIO_SYSIO4_Msk;
        // Release the JTAG ERASE pin
        MATRIX_REGS->CCFG_SYSIO |= CCFG_SYSIO_SYSIO12_Msk;
    }

    Status_code V71_EK_pin_manager::set_phase(Pin::Pin_phase phase)
    {
        if ((phase == Pin::Start_up) || (phase == Pin::Host_up))
        {
            SamV71_IO_pin::initialize_clocks();
        }
        uint8_t id_counter{};
        for (auto& pin : the_pin_table.the_pins)
        {
            if (pin.initialize(id_counter++, phase).failed())
            {
                return Status_code::Failure();
            }
        }
        return Status_code::Success();
    }

    Pin& V71_EK_pin_manager::get_pin(const Pin_id id)
    {
        if (id < Number_of_pins)
        {
            return the_pin_table.the_pins[id];
        }
        return the_pin_table.the_pins[Pin_not_used];
    }

    Pin* V71_EK_pin_manager::get_optional_pin(const Pin_id id)
    {
        if (id < Number_of_pins)
        {
            return &the_pin_table.the_pins[id];
        }
        return {};
    }

    void V71_EK_pin_manager::print_diagnostics()
    {
        printf("NORM_pin_manager\r\n");
        for (auto& pin : the_pin_table.the_pins)
        {
            if (pin.is_used())
            {
                pin.print_diagnostics();
            }
        }
    }

} // NORM_pin_manager
