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
export import Component.IDE3380_service;
import Type.Abstract_board;
import Type.Abstract_provider;
import Type.Status_code;
import Support.Configuration_repository;


export extern volatile uint8_t software_reset;

export namespace IDE3380
{
    /**
     * Facade for the IDE3380 ASICs, safe to call from any task.
     *
     * Every operation that touches SPI (or the shared register-access state) is packaged as an
     * IDE3380_request and executed by the_service task; the calling task blocks until it is done.
     * The service calls back into on_indication(), which is the only place register access happens.
     */
    class IDE3380_interface : public Abstract::Abstract_provider<IDE3380_request>
    {
        Abstract::Abstract_board& use_board;
        IDE3380_register_access the_register_access;
        IDE3380_readout_control the_readout_control{};
        IDE3380_service the_service{*this};
        uint32_t the_readback_error{};
        uint32_t the_repository_error{};
        bool the_enable_external_hold{};
        bool the_raise_external_hold{};

    public:
        static IDE3380_readout_control* optional_readout_controller;

        static void* optional_IDE3380_user;
        static wakeup_func optional_user_wakeup_func;

        explicit IDE3380_interface(Abstract::Abstract_board& board)
            : Abstract_provider(0), use_board(board), the_register_access(board)
        {
        }

        void initialize(void* user, wakeup_func wakeup, readout_func readout);

        IDE3380_readout_control& get_readout_control() { return the_readout_control; }

        [[nodiscard]] Abstract::Abstract_board& get_board() const { return use_board; }

        void start_readout() { the_readout_control.start_readout(); }

        bool continue_readout() { return the_readout_control.continue_readout(); }

        [[nodiscard]] bool is_deepsleep_ready() const { return the_readout_control.is_TXD_idle(); }

        void set_ignore_trigger(const bool flag) { the_readout_control.set_ignore_trigger(flag); }

        void raise_external_hold();

        void cease_external_hold();

        [[nodiscard]] bool is_external_hold() const { return the_raise_external_hold; }

        [[nodiscard]] uint8_t get_ASIC() const { return the_register_access.get_ASIC(); }

        // ---- Operations executed by the IDE3380 service task ----

        bool set_ASIC(uint8_t ASIC);

        void dump(const char* title);

        void print_diag();

        uint32_t SPI_read_register(uint8_t register_address);

        uint32_t SPI_update_register(uint8_t register_address, uint8_t nRead_Write, uint32_t data);

        Status_code update_registers(Repository::Configuration_repository& repository, uint32_t base_id);

        void enable_external_hold();

        void disable_external_hold();

        void override_all_thresholds();

        void disable_all_channels();

        void restore_all_channels();

        void enable_channel(uint8_t index);

        void disable_channel(uint8_t index);

        uint32_t set_channel_threshold(uint8_t index, uint8_t threshold);

        /// Called by the_service in its own task context; do not call directly.
        bool on_indication(IDE3380_request& request) override;

    private:
        bool execute(IDE3380_operation operation, uint8_t address = 0, uint8_t mode = 0, uint32_t data = 0);

        uint32_t execute_for_result(IDE3380_operation operation, uint8_t address = 0, uint8_t mode = 0,
                                    uint32_t data = 0);

        void do_dump(const char* title);

        void do_print_diag();

        void do_enable_external_hold();

        void do_disable_external_hold();

        Status_code do_update_registers(Repository::Configuration_repository& repository, uint32_t base_id);
    };
}
