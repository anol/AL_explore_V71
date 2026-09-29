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
import Component.IDE3380_interface;
import Domain.SpectraNode_keyword_lookup;
import Domain.SpectraNode_command_lookup;
import Domain.SpectraNode_provider_indication;
import Support.Configuration_repository;
import Support.Instruction_major;

using namespace IDE3380;

using namespace SpectraNode_interface;


export class Configuration_manager_provider : public Abstract_Configuration_manager_provider {
    Repository::Configuration_repository &use_repository;
    IDE3380::IDE3380_interface &use_IDE3380;
    IDE3380::IDE3380_register_access &use_IDE3380_register;

public:
    Configuration_manager_provider(Repository::Configuration_repository &repository,
                                   IDE3380::IDE3380_interface &IDE3380)
        : use_repository(repository), use_IDE3380(IDE3380), use_IDE3380_register(IDE3380.get_register_access()) {
    }

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

    void v_ASIC_asic_nbr(Instruction_major &, int asic_nbr_1) override;

    void v_ASIC_DUMP(Instruction_major &) override;

    void v_ASIC_LOAD(Instruction_major &) override;

    void v_ASIC_REG_reg_addr(Instruction_major &, int reg_addr_2) override;

    void v_ASIC_REG_reg_addr_data32(Instruction_major &, int reg_addr_1, int data32_2) override;
};
