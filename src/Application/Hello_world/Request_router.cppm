module;
#include <cstdint>
#include "Persistent_parameter_id.h"

export module Application.Request_router;
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

using namespace IDE3380;


export namespace Application
{
    class Request_router : public Abstract::Abstract_provider<Instruction_major>
    {
        Histogram_storage& use_histogram;
        IDE3380_interface& use_IDE3380;
        Calibration::Bias_calibration& use_bias;
        Repository::Configuration_repository& use_repository;
        Cadence_control& use_cadence;
        Event_counter& use_event_counter;
        Housekeeping_provider& use_housekeeper;
        Mode_control_provider& use_mode_control;
        Spectroscopic_data_provider& use_data_provider;
        Instrument_calibration_provider the_instrument_calibration{
            use_bias, use_repository, use_IDE3380, use_histogram, use_event_counter
        };
        Configuration_manager_provider the_configuration_manager{use_repository, use_IDE3380};
        int deepsleep_active{};
        int prevent_sleep{};
        int send_debug_data{};
        int deepsleep_enable{};

    public:
        explicit Request_router(Histogram_storage& histogram, Event_counter& counter,
                                IDE3380_interface& ASIC, Calibration::Bias_calibration& bias,
                                Repository::Configuration_repository& repository, Cadence_control& cadence,
                                Housekeeping_provider& housekeeper,
                                Mode_control_provider& mode_control,
                                Spectroscopic_data_provider& data_provider)
            : Abstract_provider(0),
              use_histogram(histogram), use_IDE3380(ASIC), use_bias(bias),
              use_repository(repository), use_cadence(cadence), use_event_counter(counter),
              use_housekeeper(housekeeper), use_mode_control(mode_control), use_data_provider(data_provider)
        {
        }

        void initialize();

        bool on_indication(Instruction_major& instruction) override;

        bool background_process()
        {
            return the_instrument_calibration.background_process();
        }

        void set_output_channel(const uint8_t channel)
        {
            use_data_provider.set_channel(channel);
        }

        void set_output_format(uint8_t data)
        {
            use_data_provider.set_format(static_cast<Data_format>(data));
        }

        void set_operation_mode(Operation_mode mode)
        {
            use_mode_control.set_operation_mode(mode);
        }

        void send_science_data() const
        {
            if (use_mode_control.is_nominal_mode() || use_mode_control.is_demo_mode())
            {
                const auto cadence = use_cadence.get_cadence();
                use_data_provider.send_science_data(cadence);
            }
        }

        void update_histogram(const TXD_data_t& data) const
        {
            use_histogram.update_histogram(data);
        }

        void wakeup_from_sleep()
        {
        }

        Status_code update_mode();
    };
} // Application
