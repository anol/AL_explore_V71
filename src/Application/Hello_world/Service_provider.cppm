module;
#include <cstdint>
#include "Persistent_parameter_id.h"

export module Application.Service_provider;
import Component.Histogram_storage;
import Component.IDE3380_interface;
import Component.Bias_calibration;
import Component.Event_counter;
import Type.Abstract_provider;
import Application.Cadence_control;
import Application.Configuration_manager_provider;
import Application.Housekeeping_provider;
import Application.Instrument_calibration_provider;
import Application.Mode_control_provider;
import Application.Spectroscopic_data_provider;
import Support.Configuration_repository;
import Support.Instruction_major;
import Type.Abstract_board;
import Support.Mockup_persistent_storage;
import Application.Primary_task;
import Application.Command_handler;

using namespace IDE3380;


export namespace Application
{
    class Service_provider : public Abstract::Abstract_provider<Instruction_major>
    {
        Histogram_storage& use_histogram;
        IDE3380_interface& use_IDE3380;
        Calibration::Bias_calibration& use_bias;
        Cadence_control& use_cadence;
        Event_counter& use_event_counter;
        MOCKUP::Mockup_persistent_storage the_storage{};
        Repository::Configuration_repository the_repository;
        Instrument_calibration_provider the_instrument_calibration{
            use_bias, the_repository, use_IDE3380, use_histogram, use_event_counter
        };
        Mode_control_provider the_mode_control{
            use_histogram, use_IDE3380, use_bias, the_repository, use_cadence, the_data_provider
        };
        Spectroscopic_data_provider the_data_provider{
            use_histogram, the_repository, use_cadence, use_IDE3380
        };
        Command_handler the_command_handler;
        Configuration_manager_provider the_configuration_manager;
        Housekeeping_provider the_housekeeper;
        Primary_task the_primary_task;
        int deepsleep_active{};
        int prevent_sleep{};
        int send_debug_data{};
        int deepsleep_enable{};

    public:
        explicit Service_provider(Abstract::Abstract_UART& UART, const Repository::Abstract_configuration& default_config, Histogram_storage& histogram,
                                Event_counter& counter, IDE3380_interface& ASIC, Calibration::Bias_calibration& bias,
                                Cadence_control& cadence)
            : Abstract_provider(0),
              use_histogram(histogram), use_IDE3380(ASIC), use_bias(bias),
              use_cadence(cadence), use_event_counter(counter),
              the_repository(default_config, the_storage),
              the_command_handler(histogram, ASIC, the_mode_control, the_repository),
              the_configuration_manager(the_repository, ASIC),
              the_housekeeper(UART, histogram, ASIC, bias, the_repository, cadence, the_data_provider, the_mode_control),
              the_primary_task(use_IDE3380, use_bias, the_repository, the_command_handler)
        {
        }


        bool on_indication(Instruction_major& instruction) override;

        bool background_process()
        {
            return the_instrument_calibration.background_process();
        }

        void set_output_channel(const uint8_t channel)
        {
            the_data_provider.set_channel(channel);
        }

        void set_output_format(uint8_t data)
        {
            the_data_provider.set_format(static_cast<Data_format>(data));
        }

        void set_operation_mode(Operation_mode mode)
        {
            the_mode_control.set_operation_mode(mode);
        }

        void send_science_data() const
        {
            if (the_mode_control.is_nominal_mode() || the_mode_control.is_demo_mode())
            {
                const auto cadence = use_cadence.get_cadence();
                the_data_provider.send_science_data(cadence);
            }
        }
    };
} // Application
