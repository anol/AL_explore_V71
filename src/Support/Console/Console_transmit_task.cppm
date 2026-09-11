//
// Created by aeols on 2026-09-10.
//

module;
#include "Abstract_task.h"

module Support.Console_service:Console_transmit_task;

namespace Console
{
    class Console_transmit_task : public Abstract::Abstract_task
    {
    public:
        void initialize() override;

    private:
        void task_loop();

        static void task_entry(void* object);
    };
} // Console
