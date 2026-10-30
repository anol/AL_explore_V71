//
// Created by aeols on 2026-10-30.
//

module;
#include <cstdint>
#include "Persistent_parameter_id.h"

module Application.Command_handler;

namespace Application
{
    void Command_handler::initialize()
    {
        use_IDE3380.initialize(
            this,
            [](void* user)
            {
                if (user)
                {
                    static_cast<Command_handler*>(user)->wakeup_from_sleep();
                }
            },
            [](void* user, const IDE3380::TXD_data_t& data)
            {
                if (user)
                {
                    static_cast<Command_handler*>(user)->update_histogram(data);
                }
            });
    }

    Status_code Command_handler::update_mode() const
    {
        std::int32_t value;
        const auto success{use_repository.get(Active_mode, value)};
        switch (auto mode = static_cast<Operation_mode>(value))
        {
        default:
            mode = Mode_idle;
        case Mode_idle:
            use_mode_control.set_mode(mode, Channel_idle, Format_idle, Cadence_idle);
            break;
        case Mode_nominal:
            use_mode_control.set_mode(mode, Channel_nominal, Format_nominal, Cadence_nominal);
            break;
        case Mode_demo:
            use_mode_control.set_mode(mode, Channel_demo, Format_demo, Cadence_demo);
            break;
        }
        return success;
    }
}
