//
// Created by AndersEmilOlsen on 14.04.2026.
//

#pragma once
#include <cstdint>

class Instruction_major;

namespace Abstract
{
    class Abstract_provider
    {
        const uint8_t the_key;

    public:
        explicit Abstract_provider(const uint8_t key) : the_key(key)
        {
        }

        virtual ~Abstract_provider() = default;

        virtual bool on_indication(Instruction_major&)
        {
            return false;
        }

        [[nodiscard]] uint8_t get_key() const { return the_key; }
    };
} // Request
