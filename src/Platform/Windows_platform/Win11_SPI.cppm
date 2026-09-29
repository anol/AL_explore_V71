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
* @file   Win11_SPI.cppm
* @author AndersEmilOlsen, IDEAS
* @date   22.09.2026
* @brief  
*/


export module Win11_SPI;
import Type.Abstract_SPI;
#include <cstdint>
import Platform.FreeRTOS_queue;

namespace Generic {
    class SPI_transfer_request;
}

namespace Abstract {
    class Abstract_IO_pin;
}

export namespace Win11 {
    class Win11_SPI : public Abstract::Abstract_SPI {
    public:
        Win11_SPI(uint8_t, Abstract::Abstract_IO_pin &) {
        }

        void initialize() override {
        }

        bool transfer(Abstract::Abstract_request *request) override {
            if (request) {
                auto *semaphore = request->get_semaphore();
                if (semaphore) {
                    semaphore->give();
                }
            }
            return true;
        }
    };
} // SamV71
