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
* @file   SamV71_SPI_interrupt.h
* @author AndersEmilOlsen, IDEAS
* @date   03.11.2026
* @brief
*/

module;

#include <cstdint>

export module Platform.SamV71_SPI_interrupt;
import Type.Transfer_request;

namespace SamV71 {
    class SamV71_SPI_interrupt {
        SamV71_SPI &use_driver;
        Generic::SPI_transfer_request *optional_request{};

    public:
        explicit SamV71_SPI_interrupt(SamV71_SPI &driver);

        void ISR();

    private:
        [[nodiscard]] bool is_ready() const { return optional_request == nullptr; }

        void ISR_pending_transaction(std::uint32_t &context_switch);

        void ISR_check_progress(uint32_t &context_switch);

        void ISR_transfer_complete(uint32_t &context_switch);

        void ISR_RX_ready() const;

        void ISR_TX_ready() const;
    };
} // SamV71
