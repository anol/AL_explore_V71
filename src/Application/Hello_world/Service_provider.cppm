module;
#include <cstdint>
#include "Persistent_parameter_id.h"

export module Application.Service_provider;
import Type.Abstract_provider;
import Application.Cadence_control;
import Application.Configuration_manager_provider;
import Application.Configuration_store;
import Application.Detector;
import Application.Housekeeping_provider;
import Application.Instrument_calibration_provider;
import Application.Mode_control_provider;
import Application.Spectroscopic_data_provider;
import Support.Instruction_major;
import Type.Abstract_board;
import Application.Primary_task;


export namespace Application
{
    class Service_provider : public Abstract::Abstract_provider<Instruction_major>
    {
        Configuration_store the_configuration_store{};
        Cadence_control the_cadence_control{};
        Instrument_calibration_provider the_instrument_calibration;
        Mode_control_provider the_mode_control;
        Spectroscopic_data_provider the_data_provider;
        Configuration_manager_provider the_configuration_manager;
        Housekeeping_provider the_housekeeper;
        Primary_task the_primary_task;
        int deepsleep_active{};
        int prevent_sleep{};
        int send_debug_data{};
        int deepsleep_enable{};

    public:
        Service_provider(Abstract::Abstract_UART& UART, Detector& detector)
            : Abstract_provider(0),
              the_instrument_calibration(detector.get_bias(), the_configuration_store.get_repository(),
                                         detector.get_IDE3380(), detector.get_histogram(), detector.get_event_counter()),
              the_mode_control(detector.get_histogram(), detector.get_IDE3380(), detector.get_bias(),
                               the_configuration_store.get_repository(), the_cadence_control, the_data_provider),
              the_data_provider(detector.get_histogram(), the_configuration_store.get_repository(),
                                the_cadence_control, detector.get_IDE3380()),
              the_configuration_manager(the_configuration_store.get_repository(), detector.get_IDE3380()),
              the_housekeeper(UART, detector.get_histogram(), detector.get_IDE3380(), detector.get_bias(),
                              the_configuration_store.get_repository(), the_cadence_control, the_data_provider,
                              the_mode_control),
              the_primary_task(detector.get_IDE3380(), detector.get_bias(), the_configuration_store.get_repository(),
                               detector.get_histogram(), the_mode_control)
        {
        }

        void initialize()
        {
            the_cadence_control.initialize();
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
                const auto cadence = the_cadence_control.get_cadence();
                the_data_provider.send_science_data(cadence);
            }
        }
    };
} // Application
