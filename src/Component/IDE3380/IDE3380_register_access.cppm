/*
* Copyright (C) 2026 Integrated Detector Electronics AS
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
*/

/**
* @file   IDE3380_register_access.h
* @author AndersEmilOlsen, IDEAS
* @date   13.05.2026
* @brief
*/

module;
#include <cstdint>

export module Component.IDE3380_register_access;
export import Component.IDE3380_definitions;
import Type.Abstract_SPI;
import Platform.FreeRTOS_semaphore;
import Type.Transfer_request;
import Type.Abstract_board;
import Domain.IO_pins;


export namespace IDE3380 {
    class IDE3380_register_access {
        Abstract::Abstract_board &use_board;
        Abstract::Abstract_SPI &use_SPI;
        uint8_t the_ASIC{};
        uint32_t the_channel_restore_cache[IDE3380_channel_count]{};

    public:
        static IDE3380_register_access *optional_one_and_only;

    private:
        enum {
            Number_of_ASICs = 5,
            CS_ASIC1, CS_ASIC2, CS_ASIC3, CS_ASIC4, CS_ASIC5,
            IDE3380_data_width = 40, Bits_per_byte = 8, IDE3380_data_size = IDE3380_data_width / Bits_per_byte
        };

        FreeRTOS::FreeRTOS_semaphore the_semaphore[Number_of_ASICs]{};
        Generic::SPI_transfer_request the_request[Number_of_ASICs];

    public:
        explicit IDE3380_register_access(Abstract::Abstract_board &board)
            : use_board(board), use_SPI(board.get_SPI()),
              the_request{
                  {use_board.get_pin(Domain::Pin_SPI_CS_ASIC1), the_semaphore[0], IDE3380_data_size},
                  {use_board.get_pin(Domain::Pin_SPI_CS_ASIC2), the_semaphore[1], IDE3380_data_size},
                  {use_board.get_pin(Domain::Pin_SPI_CS_ASIC3), the_semaphore[2], IDE3380_data_size},
                  {use_board.get_pin(Domain::Pin_SPI_CS_ASIC4), the_semaphore[3], IDE3380_data_size},
                  {use_board.get_pin(Domain::Pin_SPI_CS_ASIC5), the_semaphore[4], IDE3380_data_size},
              } {
        }

        void initialize();

        bool set_ASIC(uint8_t ASIC);

        [[nodiscard]] uint8_t get_ASIC() const { return the_ASIC; }

        void override_all_thresholds();

        void disable_all_channels();

        void restore_all_channels();

        void disable_channel(uint8_t index);

        void enable_channel(uint8_t index);

        uint32_t set_channel_threshold(uint8_t index, uint8_t threshold);

        void dump();

        void print_diagnostics();

        uint32_t SPI_read_register(uint8_t register_address);

        uint32_t SPI_update_register(uint8_t register_address, uint8_t nRead_Write, uint32_t data);

        uint32_t SPI_write_register(uint8_t register_address, uint32_t data);
    };
}
