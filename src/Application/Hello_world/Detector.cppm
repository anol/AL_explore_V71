export module Application.Detector;
import Component.Bias_calibration;
import Component.Bias_regulator;
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
        Histogram_storage* optional_histogram{}; // owned by the science data provider; filled by the readout

    public:
        explicit Detector(Abstract::Abstract_board& board)
            : the_IDE3380(board), the_bias_regulator(board)
        {
        }

        void initialize()
        {
            the_bias_regulator.get_calibration().initialize();
            initialize_readout();
        }

        void initialize_readout();

        void wakeup_from_sleep()
        {
        }

        IDE3380_interface& get_IDE3380() { return the_IDE3380; }

        Bias_regulator::Bias_regulator_task& get_bias_regulator() { return the_bias_regulator; }

        Calibration::Bias_calibration& get_bias() { return the_bias_regulator.get_calibration(); }


        void attach_histogram(Histogram_storage& histogram) { optional_histogram = &histogram; }
    };
} // Application
