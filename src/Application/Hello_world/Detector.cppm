export module Application.Detector;
import Component.Bias_calibration;
import Component.Bias_regulator;
import Component.Event_counter;
import Component.Histogram_storage;
import Component.IDE3380_interface;
import Type.Abstract_board;

using namespace IDE3380;

export namespace Application
{
    class Detector
    {
        IDE3380_interface the_IDE3380;
        Bias_regulator::Bias_regulator_task the_bias_regulator;
        Event_counter the_event_counter{};
        Histogram_storage the_histogram{the_event_counter, the_IDE3380};

    public:
        explicit Detector(Abstract::Abstract_board& board)
            : the_IDE3380(board), the_bias_regulator(board)
        {
        }

        void initialize()
        {
            the_event_counter.initialize();
        }

        void initialize_readout();

        void wakeup_from_sleep()
        {
        }

        IDE3380_interface& get_IDE3380() { return the_IDE3380; }

        Bias_regulator::Bias_regulator_task& get_bias_regulator() { return the_bias_regulator; }

        Calibration::Bias_calibration& get_bias() { return the_bias_regulator.get_calibration(); }

        Event_counter& get_event_counter() { return the_event_counter; }

        Histogram_storage& get_histogram() { return the_histogram; }
    };
} // Application
