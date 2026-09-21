#include <cstdio>

#include "Abstract_SPI.h"
#include "FreeRTOS_semaphore.h"
#include "Transfer_request.h"

#include "IDE3380_register_access.h"
#include "IDE3380_register_decoder.h"


namespace IDE3380 {
    IDE3380_register_access *IDE3380_register_access::optional_one_and_only{};

    void IDE3380_register_access::initialize() {
        optional_one_and_only = this;
    }

    void IDE3380_register_access::dump() {
        for (uint8_t address = 0; address < IDE3380_register_count; address++) {
            auto value = SPI_read_register(address);
            printf("0x%02X=0x%08X\r\n", address, value);
        }
    }

    void IDE3380_register_access::print_diag() {
        uint32_t value[IDE3380_register_count]{};
        for (uint8_t address = 0; address < IDE3380_register_count; address++) {
            value[address] = SPI_read_register(address);
        }
        IDE3380_register_decoder::dump_decoded(value, IDE3380_register_count);
    }

    void IDE3380_register_access::override_all_thresholds() {
        for (uint8_t index = 0; index < IDE3380_channel_count; index++) {
            the_channel_restore_cache[index] = SPI_read_register(index);
        }
        for (uint8_t index = 0; index < IDE3380_channel_count; index++) {
            auto new_value = the_channel_restore_cache[index];
            new_value      |= QC_threshold_mask; // Set max threshold
            SPI_update_register(index, IDE3380_write_only, new_value);
        }
    }

    void IDE3380_register_access::disable_all_channels() {
        for (uint8_t index = 0; index < IDE3380_channel_count; index++) {
            the_channel_restore_cache[index] = SPI_read_register(index);
        }
        for (uint8_t index = 0; index < IDE3380_channel_count; index++) {
            disable_channel(index);
        }
    }

    void IDE3380_register_access::restore_all_channels() {
        for (uint8_t index = 0; index < IDE3380_channel_count; index++) {
            SPI_update_register(index, IDE3380_write_only, the_channel_restore_cache[index]);
        }
    }

    void IDE3380_register_access::disable_channel(uint8_t index) {
        if (index < IDE3380_channel_count) {
            SPI_update_register(index, IDE3380_write_only, Disable_channel_setting);
        }
    }

    void IDE3380_register_access::enable_channel(const uint8_t index) {
        if (index < IDE3380_channel_count) {
            SPI_update_register(index, IDE3380_write_only, Enable_channel_and_max_threshold);
        }
    }

    uint32_t IDE3380_register_access::set_channel_threshold(const uint8_t index, const uint8_t threshold) {
        uint32_t value{};
        if (index < IDE3380_channel_count) {
            value = SPI_read_register(index);
            value &= ~QC_threshold_mask;            // Clear old threshold
            value |= threshold << QC_threshold_pos; // Set new threshold
            SPI_update_register(index, IDE3380_write_only, value);
        }
        return value;
    }

    uint32_t IDE3380_register_access::SPI_update_register(uint8_t register_address, uint8_t nRead_Write, uint32_t data) {
        uint32_t register_value{};
        if (register_address < IDE3380_register_count) {
            if (nRead_Write) {
                register_value = SPI_write_register(register_address, data);
            }
            if (nRead_Write != IDE3380_write_only) {
                register_value = SPI_read_register(register_address);
            }
        }
        return register_value;
    }

    uint32_t IDE3380_register_access::SPI_write_register(uint8_t register_address, uint32_t data) {
        uint32_t register_value{};
        if (register_address < IDE3380_register_count) {
            FreeRTOS::FreeRTOS_semaphore semaphore{};
            Generic::Transfer_request    request{CS_ASIC1, IDE3380_data_width, &semaphore};
            uint8_t                      data_tx[IDE3380_data_length]{};
            uint8_t                      data_rx[IDE3380_data_length]{};
            uint32_t                     valid_mask  = ~((~0) << IDE3380_register_width[register_address]);
            uint32_t                     data_masked = data & valid_mask;
            data_tx[0]                               = (register_address << 1) | 0x01; //write
            data_tx[1]                               = data_masked >> 24 & 0xFF;
            data_tx[2]                               = data_masked >> 16 & 0xFF;
            data_tx[3]                               = data_masked >> 8 & 0xFF;
            data_tx[4]                               = data_masked & 0xFF;
            request.set_buffers(data_tx, data_rx);
            if (use_SPI.transfer(&request)) {
                register_value = (data_rx[1] << 24 | data_rx[2] << 16 | data_rx[3] << 8 | data_rx[4]) & valid_mask;
            }
        }
        return register_value;
    }

    uint32_t IDE3380_register_access::SPI_read_register(uint8_t register_address) {
        uint32_t register_value{};
        if (register_address < IDE3380_register_count) {
            FreeRTOS::FreeRTOS_semaphore semaphore{};
            Generic::Transfer_request    request{CS_ASIC1, IDE3380_data_width, &semaphore};
            uint8_t                      data_tx[IDE3380_data_length]{};
            uint8_t                      data_rx[IDE3380_data_length]{};
            data_tx[0] = (register_address << 1) | 0x00; //read
            request.set_buffers(data_tx, data_rx);
            if (use_SPI.transfer(&request)) {
                uint32_t valid_mask = ~((~0) << IDE3380_register_width[register_address]);
                register_value      = (data_rx[1] << 24 | data_rx[2] << 16 | data_rx[3] << 8 | data_rx[4]) & valid_mask;
            }
        }
        return register_value;
    }
}
