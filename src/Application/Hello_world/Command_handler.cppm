//
// Created by aeols on 2026-10-30.
//

export module Application.Command_handler;
import Type.Status_code;
import Application.Mode_control_provider;
import Support.Configuration_repository;
import Component.IDE3380_interface;
import Component.IDE3380_definitions;
import Component.Histogram_storage;

namespace Application
{
    export class Command_handler
    {
        Histogram_storage& use_histogram;
        IDE3380::IDE3380_interface& use_IDE3380;
        Mode_control_provider& use_mode_control;
        Repository::Configuration_repository& use_repository;

    public:
        Command_handler(Histogram_storage& histogram, IDE3380::IDE3380_interface& IDE3380, Mode_control_provider& mode_control,
                        Repository::Configuration_repository& repository)
            : use_histogram(histogram), use_IDE3380(IDE3380), use_mode_control(mode_control), use_repository(repository)
        {
        }

        void initialize();

        [[nodiscard]] Status_code update_mode() const;

        void update_histogram(const IDE3380::TXD_data_t& data) const
        {
            use_histogram.update_histogram(data);
        }

        void wakeup_from_sleep()
        {
        }
    };
}
