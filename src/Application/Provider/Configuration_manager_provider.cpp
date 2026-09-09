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


#include "Configuration_manager_provider.h"

#include "Persistent_parameter_id.h"
#include "Configuration_repository.h"


void Configuration_manager_provider::v_CONFIG_CLEAN(Instruction_major &instruction) {
    if (use_repository.clean()) {
        instruction.print_ack();
    } else {
        instruction.print_nack(77);
    }
}

void Configuration_manager_provider::v_CONFIG_offset_data32(Instruction_major &instruction, const int offset_1,
                                                            int data32_2) {
    if (use_repository.set(offset_1, data32_2)) {
        instruction.print_ack(offset_1, data32_2);
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Configuration_manager_provider::v_CONFIG_offset(Instruction_major &instruction, const int offset_1) {
    if (int32_t value{}; use_repository.get(offset_1, value)) {
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

void Configuration_manager_provider::v_CONFIG_LOAD(Instruction_major &instruction) {
    if (use_repository.load()) {
        instruction.print_ack();
    } else {
        instruction.print_nack(88);
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
    int32_t SPI_data{};
    if (!use_repository.get(Application::IDE3380_0 + reg_addr_2, SPI_data)) {
        instruction.print_nack(use_repository.get_diag_code());
    } else {
        instruction.print_ack(reg_addr_2, SPI_data);
    }
}

void Configuration_manager_provider::v_ASIC_REG_reg_addr_data32(Instruction_major &instruction,
                                                                int reg_addr_1, int data32_2) {
    uint32_t SPI_data = use_IDE3380_register.SPI_update_register(
        reg_addr_1, IDE3380::IDE3380_write_read, data32_2);
    if (!use_repository.set(Application::IDE3380_0 + reg_addr_1, data32_2)) {
        instruction.print_nack(use_repository.get_diag_code());
    } else {
        instruction.print_ack(reg_addr_1, SPI_data);
    }
}

void Configuration_manager_provider::v_CONFIG_SAVE(Instruction_major &instruction) {
    if (use_repository.save()) {
        instruction.print_ack();
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}
