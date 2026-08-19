//
// Created by aeols on 12.08.2026.
//

#pragma once
#include "Abstract_application.h"

namespace Application {
    class Hello_world : public Abstract::Abstract_application {
    public:
        void initialize() override;

        void run() override;
    };
} // Application
