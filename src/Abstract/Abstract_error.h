//
// Created by aeols on 2026-09-10.
//

#pragma once
#include <string>

namespace Abstract
{
    class Abstract_error
    {
    public:
        virtual ~Abstract_error() = default;
        [[nodiscard]] virtual bool failed() const {return true; }
        [[nodiscard]] virtual bool success() const {return false; }
    };
} // Abstract
