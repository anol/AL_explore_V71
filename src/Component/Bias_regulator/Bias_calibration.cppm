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
        Bias_DAC the_DAC;
        Bias_ADC the_ADC;
        int32_t the_bias_25C{};
        int32_t the_temp_coeff{};
        int32_t the_bias_error{};
        volatile int32_t the_temperature{};
        volatile int32_t the_bias_volt{};

    public:
        explicit Bias_calibration(Abstract::Abstract_board &board)
            : the_DAC(board.get_DAC()), the_ADC(board.get_ADC()) {
        }

        void initialize();

        void bias_temperature_control();

        int32_t temperature_to_bias(float temperature);

        [[nodiscard]] Status_code calibrate_bias(float temperature);

        void print_diag();

        int32_t calibrateADC(int32_t raw, int32_t BIAScal35, int32_t BIAScal45, int32_t ADC_cal35, int32_t ADC_cal45);

        Bias_DAC &get_DAC() { return the_DAC; }

        Bias_ADC &get_ADC() { return the_ADC; }

        void set_setpoint_volt(const int32_t low_volt, const int32_t high_volt) {
            the_ADC.set_setpoint_volt(low_volt, high_volt);
            the_DAC.set_setpoint_volt(low_volt, high_volt);
        }

        void get_setpoint_volt(int32_t &low_volt, int32_t &high_volt) const { the_ADC.get_setpoint_volt(low_volt, high_volt); }

        void set_temperature_factors(const int32_t bias_25C, const int32_t temp_coeff) {
            the_bias_25C = bias_25C;
            the_temp_coeff = temp_coeff;
        }

        void get_temperature_factors(int32_t &bias_25C, int32_t &temp_coeff) const {
            bias_25C = the_bias_25C;
            temp_coeff = the_temp_coeff;
        }

        void set_C25_setpoint(const int32_t cal) { the_bias_25C = cal; }

        [[nodiscard]] int32_t get_C25_setpoint() const { return the_bias_25C; }

        bool override_setpoint(const int32_t setpoint) { return the_DAC.set_DAC_mV(setpoint).success(); }

        [[nodiscard]] int32_t get_setpoint() const { return the_DAC.get_DAC_mV(); }

        [[nodiscard]] int32_t get_error() const { return the_bias_error; }

        [[nodiscard]] int32_t get_temperature() const { return the_temperature; }

        void override_temperature(const int32_t temperature) { the_temperature = temperature; }

        [[nodiscard]] int32_t get_voltage() const { return the_bias_volt; }

        [[nodiscard]] uint32_t get_DAC_setting() const { return the_DAC.get_DAC_raw(); }

        [[nodiscard]] Status_code update_setpoints(Repository::Configuration_repository &repository);

        void print_status() const;

    private:
        static void hw_init_bias_regulator();
    };
}
