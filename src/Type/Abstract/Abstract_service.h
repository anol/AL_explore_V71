//
// Created by aeols on 2026-09-10.
//

#pragma once

namespace Abstract
{
    class Abstract_service
    {
    public:
        virtual ~Abstract_service() = default;
        virtual void initialize() =0;
    };
} // Abstract
