//
// Created by aeols on 12.08.2026.
//

#pragma once
#include "Abstract_application.h"
#include "Abstract_board.h"

extern "C" {
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
}

namespace Application
{
    class Hello_world : public Abstract::Abstract_application
    {
        Abstract::Abstract_board& use_board;
        TaskHandle_t the_task{};
        bool the_toggle_flag{};

    public:
        explicit Hello_world(Abstract::Abstract_board& board) : use_board(board)
        {
        }

        void initialize() override;

        void run() override;

    private:
        void task_loop();

        static void task_entry(void* object);
        void toggle_LED();
    };
} // Application
