module;

#include <cstdint>

export module Component.Bias_ADC;
import Type.Abstract_ADC;

export namespace Calibration {
    class Bias_ADC {
        Abstract::Abstract_ADC &use_ADC;
        float the_low_setpoint{};
        float the_high_setpoint{};
        float the_low_volt{};
        float the_high_volt{};
        float the_ADC_to_mV_A{};
        float the_ADC_to_mV_B{};
        static volatile int32_t the_last_sense;

    public:
        explicit Bias_ADC(Abstract::Abstract_ADC &ADC) : use_ADC(ADC) {
        }

        void initialize() {
        }

        void print_diag();

        [[nodiscard]] int32_t read_ADC_mV();

        [[nodiscard]] uint32_t read_ADC_raw() const {
            uint32_t value;
            use_ADC.get(value);
            return value;
        }

        void set_setpoints(const int32_t low, const int32_t high) {
            the_low_setpoint = low;
            the_high_setpoint = high;
            calibrate();
        }

        void get_setpoints(int32_t &low, int32_t &high) const {
            low = the_low_setpoint;
            high = the_high_setpoint;
        }

        void set_setpoint_volt(const int32_t low_volt, const int32_t high_volt) {
            the_low_volt = low_volt;
            the_high_volt = high_volt;
            calibrate();
        }

        void get_setpoint_volt(int32_t &low_volt, int32_t &high_volt) const {
            low_volt = the_low_volt;
            high_volt = the_high_volt;
        }

    private:
        void calibrate();

        [[nodiscard]] int32_t to_mV(uint32_t ADC) const;
    };
} // Temperature
