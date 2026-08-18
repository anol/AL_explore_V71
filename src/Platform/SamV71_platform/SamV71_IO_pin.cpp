/*
 * Copyright (C) 2024 Integrated Detector Electronics AS
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
 *
 */

//
// Created by aeols on 2024-08-13.
//

#include <stdio.h>

#include "SamV71_IO_pin.h"
#include "samrh71f20c.h"
#include "core_cm7.h"
#include "SamV71_clock.h"
#include "Dictionary/Generated_code/GRMU_error_codes.h"

namespace SamV71 {

    using If = Interface::IO_pin_interface;

    static inline pio_group_registers_t *register_base(const enum If::Pin_port port) {
        auto *pBlock = (pio_registers_t *) PIOA_BASE_ADDRESS;
        return &(pBlock->PIO_GROUP[port]);
    }

    SamV71_IO_pin::SamV71_IO_pin(uint8_t id, const char *name, Pin_phase phase, Pin_port port, uint8_t pin,
                                   Pin_mode mux, Pin_type type, uint8_t strength, bool default_state) :
            the_name(name),
            optional_base(register_base(port)),
            the_pin_mask(1U << pin),
            the_id(id),
            the_phase(phase),
            the_port(port),
            the_pin(pin),
            the_mode(mux),
            the_type(type),
            the_strength(strength),
            the_default_state(default_state) {}

    RMU_error_codes::Error_code SamV71_IO_pin::initialize(uint8_t id, Pin_phase phase)
    {
        if (optional_base != nullptr)
        {
            if (id != the_id) { // Assert failed: the pin id does not match
                return {RMU_error_codes::Platform_module + 0xF1};
            }
            if (!the_pin_mask) { // Assert failed: the pin mask is zero
                return {RMU_error_codes::Platform_module + 0xF2};
            }
            if (phase >= the_phase)
            {
                auto old_config = get_current_config();
                auto new_config = get_new_config(the_mode, the_type, the_strength);
                the_current_mode = the_mode;

                if (old_config != new_config)
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
                    configure_pin(new_config);
                }
            }
        }
        return {};
    }

    void SamV71_IO_pin::neutralize() {
        if (optional_base != nullptr) {
            auto neutral_config = get_new_config(If::Mode_GPIO, If::Input_pull_down, 0);
            configure_pin(neutral_config);
        }
    }

    bool SamV71_IO_pin::get() const {
        if (optional_base != nullptr) {
            return ((optional_base->PIO_PDSR) & the_pin_mask) != 0;
        } else {
            return {};
        }
    }

    void SamV71_IO_pin::set() {
        if (optional_base != nullptr) {
            optional_base->PIO_SODR = the_pin_mask;
            __DSB();
        }
    }

    void SamV71_IO_pin::clear() {
        if (optional_base != nullptr) {
            optional_base->PIO_CODR = the_pin_mask;
            __DSB();
        }
    }

    void SamV71_IO_pin::toggle() {
        if (optional_base != nullptr) {
            if (((optional_base->PIO_PDSR) & the_pin_mask) == 0) {
                optional_base->PIO_SODR = the_pin_mask;
                __DSB();
            } else {
                optional_base->PIO_CODR = the_pin_mask;
                __DSB();
            }
        }
    }

    void SamV71_IO_pin::pulse() {
        if (optional_base != nullptr) {
            optional_base->PIO_SODR = the_pin_mask;
            __DSB();
            optional_base->PIO_CODR = the_pin_mask;
            __DSB();
        }
    }

    void SamV71_IO_pin::pulse(uint32_t count) {
        if (optional_base != nullptr) {
            for (; count > 0; count--) {
                optional_base->PIO_SODR = the_pin_mask;
                __DSB();
                optional_base->PIO_CODR = the_pin_mask;
                __DSB();
            }
        }
    }


    void SamV71_IO_pin::reinitialize(Pin_mode mode)
    {
        auto old_config = get_current_config();
        auto new_config = get_new_config(mode, the_type, the_strength);

        if (old_config != new_config)
        {
            the_current_mode = mode;
            configure_pin(new_config);
        }
    }

    uint32_t SamV71_IO_pin::get_new_config(Pin_mode mux,
                                            Pin_type type,
                                            uint8_t strength) {
        uint32_t config{};
        if (optional_base != nullptr) {
            switch (type) {
                case Input_normal:
                    config |= PIO_CFGR_DIR(0);
                    break;
                case Input_pull_up:
                    config |= PIO_CFGR_DIR(0);
                    config |= PIO_CFGR_PUEN(1);
                    break;
                case Input_pull_down:
                    config |= PIO_CFGR_DIR(0);
                    config |= PIO_CFGR_PDEN(1);
                    break;
                case Input_Schmitt_trigger:
                    config |= PIO_CFGR_DIR(0);
                    config |= PIO_CFGR_SCHMITT(1);
                    break;
                case Out_normal:
                    config |= PIO_CFGR_DIR(1);
                    break;
                case Out_open_drain:
                    config |= PIO_CFGR_DIR(1);
                    config |= PIO_CFGR_OPD(1);
                    break;
                case Out_open_drain_pull_up:
                    config |= PIO_CFGR_DIR(1);
                    config |= PIO_CFGR_OPD(1);
                    config |= PIO_CFGR_PUEN(1);
                    break;
            }
            config |= PIO_CFGR_FUNC(mux);
            config |= PIO_CFGR_DRVSTR(strength);
        }
        return config;
    }

    void SamV71_IO_pin::configure_pin(uint32_t config) const {
        // Write new configuration
        optional_base->PIO_MSKR = the_pin_mask;
        __DSB();
        optional_base->PIO_CFGR = config;
        __DSB();
    }

    uint32_t SamV71_IO_pin::get_current_config() const {
        // Read current configuration
        optional_base->PIO_MSKR = the_pin_mask;
        __DSB();
        return optional_base->PIO_CFGR;
    }

    void SamV71_IO_pin::print_diagnostics() const {
        if (optional_base) {
            printf("  %-20s P%c%02d: mode=%d, type=%d, mask=0x%08lX, CFGR=0x%08lX, PDSR=%s\r\n",
                   the_name, the_port + 'A', the_pin, the_mode, the_type, the_pin_mask,
                   get_current_config(), get() ? "high" : "low");
        }
    }


    void SamV71_IO_pin::report(Service_report &report, RMU_interface::Keys as) const
    {
        if (as != RMU_interface::No_key)
        {
            report.begin_object(as);
        }
        else
        {
            report.begin_object();
        }

        if (is_used())
        {
            char port_name[2] = {static_cast<char>('A' + the_port), '\0'};
            const char *phase_name[]{"Start_up", "Host_up", "Board_up", "Detector_up", "Never_up"};
            const char *mode_name[]{"GPIO", "A", "B", "C", "D"};
            const char *type_name[]{"Input_normal", "Input_pull_up", "Input_pull_down", "Input_Schmitt_trigger",
                                    "Out_normal", "Out_open_drain", "Out_open_drain_pull_up"};

            report.report(RMU_interface::Key_info, static_cast<uint32_t>(the_id));
            report.report(RMU_interface::Key_name, the_name);
            report.report(RMU_interface::Key_memory, reinterpret_cast<uint32_t>(optional_base));
            report.report(RMU_interface::Key_channel, static_cast<uint32_t>(the_pin));
            //report.report(RMU_interface::Key_mask, the_pin_mask);
            report.report(RMU_interface::Key_pattern, phase_name[the_phase]);
            report.report(RMU_interface::Key_active, port_name);
            report.report(RMU_interface::Key_status, mode_name[the_current_mode]);
            report.report(RMU_interface::Key_type, type_name[the_type]);
            report.report(RMU_interface::Key_level, static_cast<uint32_t>(the_strength));
            report.report(RMU_interface::Key_begin, the_default_state);
            report.report(RMU_interface::Key_state, get());
        }

        report.end_object();
    }


    void SamV71_IO_pin::set_GPIO_mode()
    {
        reinitialize(Mode_GPIO);
    }


    void SamV71_IO_pin::restore_peripheral_function()
    {
        reinitialize(the_mode);
    }


    void SamV71_IO_pin::initialize_clocks() {
        Clock_interface::enable_peripheral_clock(ID_PIOA);
        Clock_interface::enable_peripheral_clock(ID_PIOB);
        Clock_interface::enable_peripheral_clock(ID_PIOC);
        Clock_interface::enable_peripheral_clock(ID_PIOD);
        Clock_interface::enable_peripheral_clock(ID_PIOE);
        Clock_interface::enable_peripheral_clock(ID_PIOF);
        Clock_interface::enable_peripheral_clock(ID_PIOG);
    }

} // SamV71
