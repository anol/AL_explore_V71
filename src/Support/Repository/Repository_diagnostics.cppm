/*
* Copyright (C) 2025-2026 Integrated Detector Electronics AS
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
* @file   Repository_diagnostics.h
* @author AndersEmilOlsen, IDEAS
* @date   03.03.2026
* @brief  
*/

module;
#include <cstdint>

export module Support.Repository_diagnostics;

export namespace Repository {
    class Repository_diagnostics {
        uint32_t the_diag_code{};

    public:
        virtual ~Repository_diagnostics() = default;

        virtual void dump() const = 0;

        virtual void print_diag() const = 0;

        void set_diag_code(const uint32_t code) { the_diag_code = code; }

        [[nodiscard]] uint32_t get_diag_code() const { return the_diag_code; };
    };
} // Repository
