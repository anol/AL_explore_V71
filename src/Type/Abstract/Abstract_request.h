//
// Created by aeols on 2026-09-21.
//

#pragma once

namespace Abstract
{
    class Abstract_semaphore;

    class Abstract_request
    {
    public:
        virtual ~Abstract_request() = default;
        [[nodiscard]] virtual Abstract_semaphore* get_semaphore() const = 0;
    };
} // Abstract
