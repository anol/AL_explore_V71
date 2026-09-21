/*
* Copyright (C) 2020-2025 Integrated Detector Electronics AS
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
* @file   Configuration_repository.h
* @author AndersEmilOlsen, IDEAS
* @date   02.02.2026
* @brief  
*/


#ifndef UNIT_TEST_CONFIGURATION_REPOSITORY_H
#define UNIT_TEST_CONFIGURATION_REPOSITORY_H
#include "Abstract_configuration.h"
#include "Persistent_storage.h"
#include "Current_configuration.h"
import Type.Status_code;

namespace Repository {
    class Configuration_repository {
        Persistent_storage &use_store;
        const Abstract_configuration &use_default_config;
        Current_configuration the_current_config{};
        uint32_t the_get_error{};
        uint32_t the_set_error{};

    public:
        Configuration_repository(const Abstract_configuration &, Persistent_storage &);

        virtual ~Configuration_repository() = default;

        void initialize();

        Status_code set(uint32_t id, int32_t value);

        [[nodiscard]] Status_code get(uint32_t id, int32_t &value);

        [[nodiscard]] Status_code clean();

        [[nodiscard]] Status_code load();

        [[nodiscard]] Status_code save();

        void dump(const char *title) const;

        virtual void print_diag() const;

        virtual uint32_t get_diag_code() { return use_store.get_diag_code(); };
    };
} // Repository

#endif //UNIT_TEST_CONFIGURATION_REPOSITORY_H
