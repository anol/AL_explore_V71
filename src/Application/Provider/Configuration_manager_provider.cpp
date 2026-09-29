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
* @file   Configuration_manager_provider.cpp
* @author AndersEmilOlsen, IDEAS
* @date   21.05.2026
* @brief  
*/

module;
#include <cstdint>
#include "Persistent_parameter_id.h"

module Application.Configuration_manager_provider;
import Component.IDE3380_interface;
import Domain.SpectraNode_keyword_lookup;
import Domain.SpectraNode_command_lookup;
import Domain.SpectraNode_provider_indication;
import Support.Configuration_repository;
import Support.Instruction_major;

using namespace IDE3380;

using namespace SpectraNode_interface;
using namespace Application;

void Configuration_manager_provider::v_CONFIG_CLEAN(Instruction_major &instruction) {
    if (use_repository.clean().success()) {
        instruction.print_ack();
    } else {
        instruction.print_nack(77);
    }
}

void Configuration_manager_provider::v_CONFIG_offset_data32(Instruction_major &instruction, const int offset_1,
                                                            int data32_2) {
    if (use_repository.set(offset_1, data32_2).success()) {
        instruction.print_ack(offset_1, data32_2);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Configuration_manager_provider::v_CONFIG_offset(Instruction_major &instruction, const int offset_1) {
    if (int32_t value{}; use_repository.get(offset_1, value).success()) {
        instruction.print_ack(offset_1, value);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Configuration_manager_provider::v_CONFIG_DUMP(Instruction_major &instruction) {
    use_repository.dump("Configuration");
    instruction.print_ack();
}

void Configuration_manager_provider::v_CONFIG_APPLY(Instruction_major &instruction) {
    use_IDE3380.update_registers(use_repository, Application::IDE3380_0);
    instruction.print_ack();
}

void Configuration_manager_provider::v_CONFIG_ASIC_asic_nbr_reg_addr_data32(Instruction_major &instruction, int asic_nbr_2, int reg_addr_3, int data32_4) {
    if (!use_repository.set(Application::IDE3380_0 + reg_addr_3, data32_4).success()) {
        instruction.print_nack(use_repository.get_diag_code());
    } else {
        instruction.print_ack(reg_addr_3, data32_4);
    }
}

void Configuration_manager_provider::v_CONFIG_ASIC_asic_nbr_reg_addr(Instruction_major &instruction, int asic_nbr_2, int reg_addr_3) {
    int32_t data{};
    if (!use_repository.get(Application::IDE3380_0 + reg_addr_3, data).success()) {
        instruction.print_nack(use_repository.get_diag_code());
    } else {
        instruction.print_ack(reg_addr_3, data);
    }
}

void Configuration_manager_provider::v_CONFIG_LOAD(Instruction_major &instruction) {
    if (use_repository.load().success()) {
        instruction.print_ack();
    } else {
        instruction.print_nack(88);
    }
}

void Configuration_manager_provider::v_ASIC(Instruction_major &instruction) {
    instruction.print_ack(use_IDE3380.get_register_access().get_ASIC());
}

void Configuration_manager_provider::v_ASIC_asic_nbr(Instruction_major &instruction, int asic_nbr_1) {
    asic_nbr_1--;
    if (use_IDE3380.get_register_access().set_ASIC(asic_nbr_1)) {
        instruction.print_ack(use_IDE3380.get_register_access().get_ASIC());
    } else {
        instruction.print_nack(99);
    }
}

void Configuration_manager_provider::v_ASIC_DUMP(Instruction_major &instruction) {
    use_IDE3380.dump("ASIC Registers");
    instruction.print_ack();
}

void Configuration_manager_provider::v_ASIC_LOAD(Instruction_major &instruction) {
    use_IDE3380.update_registers(use_repository, Application::IDE3380_0);
    instruction.print_ack();
}

void Configuration_manager_provider::v_ASIC_REG_reg_addr(Instruction_major &instruction, int reg_addr_2) {
    const int SPI_data = use_IDE3380_register.SPI_read_register(reg_addr_2);
    instruction.print_ack(reg_addr_2, SPI_data);
}

void Configuration_manager_provider::v_ASIC_REG_reg_addr_data32(Instruction_major &instruction,
                                                                int reg_addr_1, int data32_2) {
    const int SPI_data = use_IDE3380_register.SPI_update_register(
        reg_addr_1, IDE3380::IDE3380_write_read, data32_2);
    if (SPI_data == data32_2) {
        instruction.print_ack(reg_addr_1, SPI_data);
    } else {
        instruction.print_nack(SPI_data);
    }
}

void Configuration_manager_provider::v_CONFIG_SAVE(Instruction_major &instruction) {
    if (use_repository.save().success()) {
        instruction.print_ack();
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}
