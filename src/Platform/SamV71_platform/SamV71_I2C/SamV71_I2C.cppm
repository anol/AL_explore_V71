//
// Created by aeols on 2026-11-19.
//

module;

#include <cstdint>

export module Platform.SamV71_I2C;
import :Config;
import :Driver;
import :ISR;

import Type.Abstract_I2C;
import Type.I2C_transfer_request;
import Platform.FreeRTOS_semaphore;
import Platform.SamV71_clock;

export namespace SamV71
{
    /**
     * I2C host on a TWIHS peripheral (SDA/SCL pin muxing is done by the board pin table).
     *
     * device_id is the 7-bit device address; data_width is 1..4 bytes, carried MSB first on the bus.
     * Each read() / write() is one START ... STOP frame; the calling task blocks until it is done.
     */
    class SamV71_I2C : public Abstract::Abstract_I2C
    {
        const uint8_t the_id;
        SamV71_I2C_driver the_driver;
        SamV71_I2C_ISR the_ISR;

    public:
        explicit SamV71_I2C(uint8_t id)
            : the_id(id),
              the_driver(SamV71_I2C_config::get_config(id)),
              the_ISR(SamV71_I2C_config::get_config(id), the_driver)
        {
        }

        void initialize() override;

        bool read(uint8_t device_id, uint8_t data_width, uint32_t& data) override;

        bool write(uint8_t device_id, uint8_t data_width, uint32_t data) override;

        void print_diagnostics() const { the_driver.print_diagnostics(); }

        [[nodiscard]] uint8_t get_id() const { return the_id; }

    private:
        static bool is_valid_width(const uint8_t data_width)
        {
            return data_width > 0 && data_width <= Generic::I2C_transfer_request::Max_data_size;
        }
    };

    void SamV71_I2C::initialize()
    {
        if (const auto* config = SamV71_I2C_config::get_config(the_id))
        {
            SamV71_clock::enable_peripheral_clock(config->the_peripheral_id);
            the_driver.disable_I2C();
            the_driver.setup_I2C_registers();
            the_driver.enable_I2C();
        }
    }

    bool SamV71_I2C::read(const uint8_t device_id, const uint8_t data_width, uint32_t& data)
    {
        if (!is_valid_width(data_width)) { return false; }
        FreeRTOS::FreeRTOS_semaphore semaphore{};
        Generic::I2C_transfer_request request{semaphore, device_id, true, data_width};
        const bool success = the_driver.transfer(&request) && request.is_success();
        if (success) { data = request.get_value(); }
        return success;
    }

    bool SamV71_I2C::write(const uint8_t device_id, const uint8_t data_width, const uint32_t data)
    {
        if (!is_valid_width(data_width)) { return false; }
        FreeRTOS::FreeRTOS_semaphore semaphore{};
        Generic::I2C_transfer_request request{semaphore, device_id, false, data_width, data};
        return the_driver.transfer(&request) && request.is_success();
    }
} // SamV71
