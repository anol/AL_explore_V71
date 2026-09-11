/*
* Copyright (C) 2020-2026 Integrated Detector Electronics AS
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
* @file   Persistent_storage.h
* @author AndersEmilOlsen, IDEAS
* @date   02.02.2026
* @brief  
*/


#ifndef UNIT_TEST_PERSISTENT_STORAGE_H
#define UNIT_TEST_PERSISTENT_STORAGE_H

#include <cstdint>

#include "Repository_diagnostics.h"
#include "Status_code.h"

namespace Repository {
    class Persistent_storage : public Repository_diagnostics {
    public:
        ~Persistent_storage() override = default;

        [[nodiscard]] virtual Status_code initialize() = 0;

        [[nodiscard]] virtual Status_code clean() = 0;

        [[nodiscard]] virtual Status_code open_reading() = 0;

        [[nodiscard]] virtual Status_code read(uint32_t id, int32_t &value) = 0;

        [[nodiscard]] virtual Status_code open_writing(int dirty_count) = 0;

        [[nodiscard]] virtual Status_code write_cache(uint32_t id, int32_t value) = 0;

        [[nodiscard]] virtual Status_code program_flash() = 0;
    };
} // Repository

#endif //UNIT_TEST_PERSISTENT_STORAGE_H
