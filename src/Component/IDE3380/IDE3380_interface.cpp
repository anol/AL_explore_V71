/*
 * IDE3380.c
 *
 *  Created on: Sep 17, 2024
 *      Author: Daniel
 */

module;
#include <cstdint>
#include <cstdio>

module Component.IDE3380_interface;
import Component.IDE3380_definitions;
import Component.IDE3380_readout_control;
import Component.IDE3380_register_access;
import Type.Abstract_board;
import Type.Status_code;
import Support.Configuration_repository;
import Component.IDE3380_register_decoder;
import Component.IDE3380_service;
import Domain.Keyword_lookup;
import Support.Instruction_major;
import Support.Token_type;

volatile uint8_t software_reset = 0;

// extern "C" void HAL_GPIO_EXTI_Rising_Callback(const uint16_t GPIO_Pin) {
//     using namespace IDE3380;
//     if (IDE3380_interface::optional_readout_controller) {
//         IDE3380_interface::optional_readout_controller->on_GPIO_interrupt(GPIO_Pin);
//     }
//     if (GPIO_Pin == BOOT0_Pin || GPIO_Pin == EXT_WAKEUP_Pin) //GPIO_Pin == EXT_WAKEUP_Pin ||
//     {
//         //external interrupt from user to exit deep sleep mode and resume normal operation mode to update registers
//         //tell super-loop to not enter deep sleep mode again
//         HAL_PWR_DisableSleepOnExit();
//         SystemClock_Config();
//         HAL_ResumeTick();
//         if (IDE3380_interface::optional_user_wakeup_func) {
//             IDE3380_interface::optional_user_wakeup_func(IDE3380_interface::optional_IDE3380_user);
//         }
//         HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
//         //When entering STOP2 mode the UART is clocked off so after the Wake-up you need to re-clock the UART, then will work well
//         STM32U575RG::U575xx_USB_serial::open_rx();
//         software_reset = 1;
//     }
// }

namespace IDE3380 {
    void *IDE3380_interface::optional_IDE3380_user{};
    wakeup_func IDE3380_interface::optional_user_wakeup_func{};
    IDE3380_readout_control *IDE3380_interface::optional_readout_controller{};

    void IDE3380_interface::initialize(void *user, const wakeup_func wakeup, const readout_func readout) {
        optional_IDE3380_user = user;
        optional_user_wakeup_func = wakeup;
        optional_readout_controller = &the_readout_control;
        the_readout_control.initialize(user, readout);
        the_register_access.initialize();
    }

    void IDE3380_interface::do_dump() {
        the_register_access.dump();
    }

    void IDE3380_interface::do_print_diag() {
        printf("ASIC: get_err=%d, load_err=%d\r\n", the_repository_error, the_readback_error);
        the_readout_control.print_diag();
        the_register_access.print_diagnostics();
    }

    void IDE3380_interface::do_enable_external_hold() {
        the_register_access.SPI_update_register(IDE3380_readout_mode_reg, IDE3380_write_read, Enable_external_hold);
        the_enable_external_hold = true;
    }

    void IDE3380_interface::do_disable_external_hold() {
        the_register_access.SPI_update_register(IDE3380_readout_mode_reg, IDE3380_write_read, Disable_external_hold);
        the_enable_external_hold = false;
    }

    void IDE3380_interface::raise_external_hold() {
        // HAL_GPIO_WritePin(IDE3380_HOLD_GPIO_Port, IDE3380_HOLD_Pin, GPIO_PIN_SET);
        the_raise_external_hold = true;
    }

    void IDE3380_interface::cease_external_hold() {
        // HAL_GPIO_WritePin(IDE3380_HOLD_GPIO_Port, IDE3380_HOLD_Pin, GPIO_PIN_RESET);
        the_raise_external_hold = false;
    }

    Status_code IDE3380_interface::do_update_registers(const uint32_t base_id) {
        if (optional_repository == nullptr) {
            printf("<> IDE3380 update registers failed: no repository attached <>\r\n");
            return Status_code::Failure();
        }
        auto &repository = *optional_repository;
        auto success{true};
        int update_count{};
        for (uint8_t address = IDE3380_channel_1_reg; address <= IDE3380_sysclock_control_reg; address++) {
            const auto id = base_id + address;
            if (int32_t value; repository.get(id, value).success()) {
                const auto readback = static_cast<int32_t>(
                    the_register_access.SPI_update_register(address, IDE3380_write_read, value));
                if (value != readback) {
                    if (success) {
                        success = false;
                        printf("<> IDE3380 readback error. Diag: expected=0x%08X, actual=0x%08X .....\r\n",
                               value, readback);
                    }
                    the_readback_error++;
                } else {
                    update_count++;
                }
            } else {
                success = false;
                the_repository_error++;
            }
        }
        if (success) {
            printf("Updated %d IDE3380 registers\r\n", update_count);
        } else {
            printf("<> IDE3380 update registers failed. Error accum.: readback=%d, repository=%d <>\r\n",
                   the_readback_error, the_repository_error);
        }
        return Status_code(success);
    }

    // ---- Instruction helpers ----

    namespace {
        using namespace Domain_interface;

        bool has_key(const Instruction_major &instruction, const uint8_t key) {
            for (uint32_t index = 0; index < instruction.get_number_of_tokens(); index++) {
                if (instruction.get_token_id(index) == key) { return true; }
            }
            return false;
        }

        /// Value of the first non-keyword token with this id, or fallback when absent.
        uint32_t value_of(const Instruction_major &instruction, const uint8_t key, const uint32_t fallback = 0) {
            for (uint32_t index = 0; index < instruction.get_number_of_tokens(); index++) {
                if (instruction.get_token_id(index) == key && instruction.get_token_type(index) !=
                    Instruction::Keyword_token) {
                    return static_cast<uint32_t>(instruction.get_token_integer(index));
                }
            }
            return fallback;
        }
    }

    // ---- Client side: build the instruction and hand it to the service task ----

    Instruction_major IDE3380_interface::make_request(const uint8_t operation, const uint8_t sub_operation) {
        Instruction_major instruction{};
        instruction.set_request(Key_ASIC);
        if (operation) { instruction.add_command(operation); }
        if (sub_operation) { instruction.add_command(sub_operation); }
        return instruction;
    }

    bool IDE3380_interface::send(const uint8_t operation, const uint8_t sub_operation) {
        auto instruction = make_request(operation, sub_operation);
        return the_service.execute(instruction);
    }

    uint32_t IDE3380_interface::send_for_result(Instruction_major &instruction) {
        the_service.execute(instruction);
        return value_of(instruction, Key_result);
    }

    bool IDE3380_interface::set_ASIC(const uint8_t ASIC) {
        auto instruction = make_request(0);
        instruction.add_value(Key_asic_nbr, ASIC);
        return the_service.execute(instruction);
    }

    void IDE3380_interface::dump(const char *title) {
        if (title) { printf("%s\r\n", title); }
        send(Key_DUMP);
    }

    void IDE3380_interface::print_diag() { send(Key_DIAG); }

    uint32_t IDE3380_interface::SPI_read_register(const uint8_t register_address) {
        auto instruction = make_request(Key_REG);
        instruction.add_value(Key_reg_addr, register_address);
        return send_for_result(instruction);
    }

    uint32_t IDE3380_interface::SPI_update_register(const uint8_t register_address, const uint8_t nRead_Write,
                                                    const uint32_t data) {
        auto instruction = make_request(Key_REG);
        instruction.add_value(Key_reg_addr, register_address);
        instruction.add_value(Key_data32, data);
        instruction.add_value(Key_access, nRead_Write);
        return send_for_result(instruction);
    }

    Status_code IDE3380_interface::update_registers(const uint32_t base_id) {
        auto instruction = make_request(Key_LOAD);
        instruction.add_value(Key_offset, base_id);
        return Status_code(the_service.execute(instruction));
    }

    void IDE3380_interface::enable_external_hold() { send(Key_HOLD, Key_ENABLE); }

    void IDE3380_interface::disable_external_hold() { send(Key_HOLD, Key_DISABLE); }

    void IDE3380_interface::override_all_thresholds() { send(Key_THRESHOLD, Key_OVERRIDE); }

    void IDE3380_interface::disable_all_channels() { send(Key_CHANNEL, Key_DISABLE); }

    void IDE3380_interface::restore_all_channels() { send(Key_CHANNEL, Key_RESTORE); }

    void IDE3380_interface::enable_channel(const uint8_t index) {
        auto instruction = make_request(Key_CHANNEL, Key_ENABLE);
        instruction.add_value(Key_channel, index);
        the_service.execute(instruction);
    }

    void IDE3380_interface::disable_channel(const uint8_t index) {
        auto instruction = make_request(Key_CHANNEL, Key_DISABLE);
        instruction.add_value(Key_channel, index);
        the_service.execute(instruction);
    }

    uint32_t IDE3380_interface::set_channel_threshold(const uint8_t index, const uint8_t threshold) {
        auto instruction = make_request(Key_CHANNEL, Key_THRESHOLD);
        instruction.add_value(Key_channel, index);
        instruction.add_value(Key_data8, threshold);
        return send_for_result(instruction);
    }

    // ---- Service side: runs in the IDE3380 service task only ----

    bool IDE3380_interface::on_indication(Instruction_major &instruction) {
        if (instruction.get_token_id(0) != Key_ASIC) { return false; }
        switch (instruction.get_token_id(1)) {
            case Key_asic_nbr:
                return the_register_access.set_ASIC(static_cast<uint8_t>(value_of(instruction, Key_asic_nbr)));
            case Key_REG:
                return on_register(instruction);
            case Key_LOAD:
                return do_update_registers(value_of(instruction, Key_offset)).success();
            case Key_DUMP:
                do_dump();
                return true;
            case Key_DIAG:
                do_print_diag();
                return true;
            case Key_HOLD:
                if (has_key(instruction, Key_ENABLE)) {
                    do_enable_external_hold();
                    return true;
                }
                if (has_key(instruction, Key_DISABLE)) {
                    do_disable_external_hold();
                    return true;
                }
                return false;
            case Key_THRESHOLD:
                if (has_key(instruction, Key_OVERRIDE)) {
                    the_register_access.override_all_thresholds();
                    return true;
                }
                return false;
            case Key_CHANNEL:
                return on_channel(instruction);
            default:
                return false;
        }
    }

    bool IDE3380_interface::on_register(Instruction_major &instruction) {
        if (!has_key(instruction, Key_reg_addr)) { return false; }
        const auto address = static_cast<uint8_t>(value_of(instruction, Key_reg_addr));
        uint32_t result;
        if (has_key(instruction, Key_data32)) {
            const auto access = static_cast<uint8_t>(value_of(instruction, Key_access, IDE3380_write_read));
            result = the_register_access.SPI_update_register(address, access, value_of(instruction, Key_data32));
        } else {
            result = the_register_access.SPI_read_register(address);
        }
        instruction.add_value(Key_result, result);
        return true;
    }

    bool IDE3380_interface::on_channel(Instruction_major &instruction) {
        auto &access = the_register_access;
        const bool has_channel = has_key(instruction, Key_channel);
        const auto index = static_cast<uint8_t>(value_of(instruction, Key_channel));
        if (has_key(instruction, Key_RESTORE)) {
            access.restore_all_channels();
        } else if (has_key(instruction, Key_ENABLE) && has_channel) {
            access.enable_channel(index);
        } else if (has_key(instruction, Key_DISABLE)) {
            if (has_channel) { access.disable_channel(index); } else { access.disable_all_channels(); }
        } else if (has_key(instruction, Key_THRESHOLD) && has_channel && has_key(instruction, Key_data8)) {
            const auto threshold = static_cast<uint8_t>(value_of(instruction, Key_data8));
            instruction.add_value(Key_result, access.set_channel_threshold(index, threshold));
        } else {
            return false;
        }
        return true;
    }
}
