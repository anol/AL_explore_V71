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
* @file   Configuration_manager_provider.h
* @author AndersEmilOlsen, IDEAS
* @date   21.05.2026
* @brief
*/

module;
#include "Persistent_parameter_id.h"
#include <cstdint>

export module Application.Configuration_manager_provider;
import Component.IDE3380_provider;
import Domain.Keyword_lookup;
import Application.SpectraNode_command_lookup;
import Application.SpectraNode_provider_indication;
import Support.Configuration_repository;
import Application.Configuration_store;
import Support.Instruction_major;

using namespace IDE3380;

using namespace SpectraNode_interface;


export class Configuration_manager_provider : public Abstract_Configuration_manager_provider {
    Application::Configuration_store the_configuration_store{};
    IDE3380::IDE3380_provider &use_IDE3380;

public:
    explicit Configuration_manager_provider(IDE3380::IDE3380_provider &IDE3380)
        : use_IDE3380(IDE3380) {
    }

    Repository::Configuration_repository &get_repository() { return the_configuration_store.get_repository(); }

protected:
    void v_CONFIG_CLEAN(Instruction_major &) override;

    void v_CONFIG_offset_data32(Instruction_major &, int offset_1, int data32_2) override;

    void v_CONFIG_offset(Instruction_major &, int offset_1) override;

    void v_CONFIG_LOAD(Instruction_major &) override;

    void v_CONFIG_SAVE(Instruction_major &) override;

    void v_CONFIG_DUMP(Instruction_major &) override;

    void v_CONFIG_APPLY(Instruction_major &) override;

    void v_CONFIG_ASIC_asic_nbr_reg_addr_data32(Instruction_major &, int asic_nbr_2, int reg_addr_3, int data32_4) override;

    void v_CONFIG_ASIC_asic_nbr_reg_addr(Instruction_major &, int asic_nbr_2, int reg_addr_3) override;

    void v_ASIC(Instruction_major &) override;

    void v_ASIC_asic_nbr(Instruction_major &, int asic_nbr_1) override;

    void v_ASIC_DUMP(Instruction_major &) override;

    void v_ASIC_LOAD(Instruction_major &) override;

    void v_ASIC_REG_reg_addr(Instruction_major &, int reg_addr_2) override;

    void v_ASIC_REG_reg_addr_data32(Instruction_major &, int reg_addr_1, int data32_2) override;
};
