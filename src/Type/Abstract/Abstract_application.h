//
// Created by aeols on 12.08.2026.
//

#pragma once

namespace Abstract {
    class Abstract_application {
    public:
        Abstract_application() = default;

        virtual ~Abstract_application() = default;

        virtual void initialize() = 0;

        virtual void run() =0;
    };
} // Abstract
