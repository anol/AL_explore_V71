//
// Created by anolsen on 11.02.2020.
//
#ifndef INTERFACE_BOARD_FULL_INTERFACE_H
#define INTERFACE_BOARD_FULL_INTERFACE_H

#include "Timer_interface.h"
#include "Board_utility_interface.h"

class ADC_interface;

class Dosimeter_ADC_interface;

class I2C_interface;

class SPI_interface;

namespace Interface {

    /// Purpose: Hardware abstraction of the board support package.
    class Board_full_interface : public Interface::Board_utility_interface {
    public:
        explicit Board_full_interface(NORM_bookkeeping::Bookkeeper_role role) : Interface::Board_utility_interface(role) {}

        /// @note cp: support for SW1040-GNORM-ESW/issues/4:
        /// moved to utility interface to allow board state reporting from system task
        //virtual void report_state(Service_report *) const = 0;

        virtual bool read_temperature() = 0;

        virtual bool get_temperature(uint32_t &temp_A, uint32_t &temp_B) = 0;

        virtual const char *get_board_name() const = 0;

        virtual bool is_idle() { return true; }

        virtual ADC_interface &get_ADC() = 0;

        virtual Dosimeter_ADC_interface &get_dosimeter_ADC() = 0;

        virtual I2C_interface &get_I2C() = 0;

        virtual SPI_interface &get_SPI() = 0;

        virtual Timer_interface &get_timer(Timer_interface::Timer_function timer_function) = 0;

        virtual Host_bus::Bus_manager &get_bus_manager() = 0;

        virtual void set_external_trigger(bool lowgain) = 0;

        virtual bool get_external_trigger_low() const = 0;
    };

} // Interface

#endif //INTERFACE_BOARD_FULL_INTERFACE_H
