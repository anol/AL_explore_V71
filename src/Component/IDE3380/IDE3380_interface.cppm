/*
 * IDE3380.h
 *
 *  Created on: Sep 17, 2024
 *      Author: Daniel
 */

module;
#include <cstdint>

export module Component.IDE3380_interface;
export import Component.IDE3380_definitions;
export import Component.IDE3380_readout_control;
export import Component.IDE3380_register_access;
import Type.Abstract_board;
import Type.Status_code;
import Support.Configuration_repository;


export extern volatile uint8_t software_reset;

export namespace IDE3380 {
    class IDE3380_interface {
        Abstract::Abstract_board &use_board;
        IDE3380_register_access   the_register_access;
        IDE3380_readout_control   the_readout_control{};
        uint32_t                  the_readback_error{};
        uint32_t                  the_repository_error{};
        bool                      the_enable_external_hold{};
        bool                      the_raise_external_hold{};

    public:
        static IDE3380_readout_control *optional_readout_controller;

        static void *      optional_IDE3380_user;
        static wakeup_func optional_user_wakeup_func;

        explicit IDE3380_interface(Abstract::Abstract_board &board) : use_board(board), the_register_access(board.get_SPI()) {
        }

        void initialize(void *user, wakeup_func wakeup, readout_func readout);

        IDE3380_register_access &get_register_access() { return the_register_access; }

        IDE3380_readout_control &get_readout_control() { return the_readout_control; }

        void dump(const char *str);

        void print_diag();

        void start_readout() { the_readout_control.start_readout(); }

        bool continue_readout() { return the_readout_control.continue_readout(); }

        [[nodiscard]] bool is_deepsleep_ready() const { return the_readout_control.is_TXD_idle(); }

        void enable_external_hold();

        void disable_external_hold();

        void raise_external_hold();

        void cease_external_hold();

        [[nodiscard]] bool is_external_hold() const { return the_raise_external_hold; }

        Status_code update_registers(Repository::Configuration_repository &repository, uint32_t base_id);
    };
}
