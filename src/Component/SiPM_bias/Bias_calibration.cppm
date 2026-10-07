/*
 * Temperature.h
 *
 *  Created on: Feb 18, 2025
 *      Author: Daniel
 */

module;
#include <cstdint>

export module Component.Bias_calibration;
import Component.Bias_ADC;
import Component.Bias_DAC;
import Support.Configuration_repository;
import Type.Abstract_board;


export namespace Calibration {
    class Bias_calibration {
        int32_t the_bias_at_25{};
        volatile int16_t the_temperature{};
        volatile int32_t the_bias_volt{};
        float gamma_param_A = 1;
        float gamma_param_B = 0;
        Bias_DAC the_DAC;
        Bias_ADC the_ADC{};

    public:
        explicit Bias_calibration(Abstract::Abstract_board &board) : the_DAC(board.get_DAC()) {
        }

        void initialize();

        void bias_temperature_control();

        int32_t temperature_to_bias(float temperature);

        [[nodiscard]] Status_code calibrate_bias(float temperature);

        void set_calibration(int32_t DAC_35V, int32_t DAC_45V, int32_t ADC_35V, int32_t ADC_45V, int32_t bias_25C);

        void print_diag();

        int32_t calibrateADC(int32_t raw, int32_t BIAScal35, int32_t BIAScal45, int32_t ADC_cal35, int32_t ADC_cal45);

        Bias_DAC &get_DAC() { return the_DAC; }

        Bias_ADC &get_ADC() { return the_ADC; }

        void set_bias_25C(const int32_t cal) { the_bias_at_25 = cal; }

        [[nodiscard]] int32_t get_temperature() const { return the_temperature; }

        [[nodiscard]] int32_t get_voltage() const { return the_bias_volt; }

        [[nodiscard]] uint32_t get_DAC_setting() const { return the_DAC.get_DAC_setting(); }

        [[nodiscard]] Status_code update_setpoints(Repository::Configuration_repository &repository);

        void set_gamma_param_A(const float x) { gamma_param_A = x; }

        void set_gamma_param_B(const float x) { gamma_param_B = x; }

        void print_status() const;

    private:
        static void hw_init_bias_regulator();
    };
}
