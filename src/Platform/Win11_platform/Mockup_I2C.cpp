module;
#include <cstdint>
#include <cstdio>

module Platform.Mockup_I2C;

namespace Mockup {
    bool Mockup_I2C::read(const uint8_t device_id, const uint8_t data_width, uint32_t &data) {
        auto *device = data_width > 0 && data_width <= Max_data_width ? find_device(device_id) : nullptr;
        if (device == nullptr) {
            the_failure_count++;
            return false;
        }
        uint32_t value{};
        for (uint8_t index = 0; index < data_width; index++) {
            value = value << 8 | device->the_register[device->the_pointer++];
        }
        data = value;
        the_read_count++;
        return true;
    }

    bool Mockup_I2C::write(const uint8_t device_id, const uint8_t data_width, const uint32_t data) {
        auto *device = data_width > 0 && data_width <= Max_data_width ? find_device(device_id) : nullptr;
        if (device == nullptr) {
            the_failure_count++;
            return false;
        }
        for (uint8_t index = 0; index < data_width; index++) {
            const auto byte = static_cast<uint8_t>(data >> 8 * (data_width - 1 - index));
            if (index == 0) {
                device->the_pointer = byte;
            } else {
                device->the_register[device->the_pointer++] = byte;
            }
        }
        the_write_count++;
        return true;
    }

    Mockup_I2C::Device *Mockup_I2C::find_device(uint8_t device_id) {
        device_id &= 0x7F;
        for (auto &device: the_device) {
            if (device.is_used && device.the_address == device_id) {
                return &device;
            }
        }
        for (auto &device: the_device) {
            if (!device.is_used) {
                device.is_used = true;
                device.the_address = device_id;
                return &device;
            }
        }
        return nullptr;
    }

    void Mockup_I2C::print_diagnostics() const {
        int devices{};
        for (const auto &device: the_device) {
            devices += device.is_used;
        }
        printf("I2C (mockup): devices=%d, reads=%u, writes=%u, failures=%u\r\n", devices,
               static_cast<unsigned>(the_read_count), static_cast<unsigned>(the_write_count),
               static_cast<unsigned>(the_failure_count));
    }
} // Mockup
