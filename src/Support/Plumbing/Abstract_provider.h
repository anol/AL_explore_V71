//
// Created by AndersEmilOlsen on 14.04.2026.
//

#pragma once

namespace Instruction {
    class Abstract_provider {
    public:
        explicit Abstract_provider(uint8_t) {
        }

        virtual ~Abstract_provider() = default;

        virtual bool on_indication(Instruction_major &) {
            return false;
        }
    };
} // Request
