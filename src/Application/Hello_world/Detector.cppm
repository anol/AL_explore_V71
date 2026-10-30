export module Application.Detector;
import Component.Bias_calibration;
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
        Calibration::Bias_calibration the_bias;
        Event_counter the_event_counter{};
        Histogram_storage the_histogram{the_event_counter, the_IDE3380};

    public:
        explicit Detector(Abstract::Abstract_board& board)
            : the_IDE3380(board), the_bias(board)
        {
        }

        void initialize()
        {
            the_event_counter.initialize();
        }

        IDE3380_interface& get_IDE3380() { return the_IDE3380; }

        Calibration::Bias_calibration& get_bias() { return the_bias; }

        Event_counter& get_event_counter() { return the_event_counter; }

        Histogram_storage& get_histogram() { return the_histogram; }
    };
} // Application
