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

namespace IDE3380
{
    void* IDE3380_interface::optional_IDE3380_user{};
    wakeup_func IDE3380_interface::optional_user_wakeup_func{};
    IDE3380_readout_control* IDE3380_interface::optional_readout_controller{};

    void IDE3380_interface::initialize(void* user, const wakeup_func wakeup, const readout_func readout)
    {
        optional_IDE3380_user = user;
        optional_user_wakeup_func = wakeup;
        optional_readout_controller = &the_readout_control;
        the_readout_control.initialize(user, readout);
        the_register_access.initialize();
    }

    void IDE3380_interface::dump(const char* title)
    {
        printf("%s\r\n", title);
        the_register_access.dump();
    }

    void IDE3380_interface::print_diag()
    {
        printf("ASIC: get_err=%d, load_err=%d\r\n", the_repository_error, the_readback_error);
        the_readout_control.print_diag();
        the_register_access.print_diagnostics();
    }

    void IDE3380_interface::enable_external_hold()
    {
        the_register_access.SPI_update_register(IDE3380_readout_mode_reg, IDE3380_write_read, Enable_external_hold);
        the_enable_external_hold = true;
    }

    void IDE3380_interface::disable_external_hold()
    {
        the_register_access.SPI_update_register(IDE3380_readout_mode_reg, IDE3380_write_read, Disable_external_hold);
        the_enable_external_hold = false;
    }

    void IDE3380_interface::raise_external_hold()
    {
        // HAL_GPIO_WritePin(IDE3380_HOLD_GPIO_Port, IDE3380_HOLD_Pin, GPIO_PIN_SET);
        the_raise_external_hold = true;
    }

    void IDE3380_interface::cease_external_hold()
    {
        // HAL_GPIO_WritePin(IDE3380_HOLD_GPIO_Port, IDE3380_HOLD_Pin, GPIO_PIN_RESET);
        the_raise_external_hold = false;
    }

    Status_code IDE3380_interface::update_registers(Repository::Configuration_repository& repository,
                                                    const uint32_t base_id)
    {
        auto success{true};
        for (uint8_t address = IDE3380_channel_1_reg; address <= IDE3380_sysclock_control_reg; address++)
        {
            const auto id = base_id + address;
            if (int32_t value; repository.get(id, value).success())
            {
                const auto readback = static_cast<int32_t>(
                    the_register_access.SPI_update_register(address, IDE3380_write_read, value));
                if (value != readback)
                {
                    if (success)
                    {
                        success = false;
                        printf("<> IDE3380 readback error. Diag: expected=0x%08X, actual=0x%08X .....\r\n",
                               value, readback);
                    }
                    the_readback_error++;
                }
            }
            else
            {
                success = false;
                the_repository_error++;
            }
        }
        if (!success)
        {
            printf("<> IDE3380 update registers failed. Error accum.: readback=%d, repository=%d <>\r\n",
                   the_readback_error, the_repository_error);
        }
        return Status_code(success);
    }
}
