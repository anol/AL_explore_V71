module;
#include <cstdint>
#include <cstdio>

module Platform.Mockup_SPI;
import Type.Transfer_request;

namespace Mockup {
    bool Mockup_SPI::transfer(Abstract::Abstract_request *request) {
        auto *spi_request = dynamic_cast<Generic::SPI_transfer_request *>(request);
        if (spi_request == nullptr) {
            the_failure_count++;
            return false;
        }
        auto &chip_select = spi_request->get_chip_select();
        auto *device = find_device(chip_select);
        if (device == nullptr) {
            the_failure_count++;
            return false;
        }
        chip_select.clear();

        // Clock out the whole frame.
        uint8_t frame[Max_frame_size]{};
        const auto *transmit = spi_request->get_transmit_buffer();
        while (spi_request->is_more_to_send()) {
            const auto index = spi_request->get_send_count();
            const uint8_t data = transmit ? transmit[index] : 0xFF;
            if (index < Max_frame_size) {
                frame[index] = data;
            }
            spi_request->count_sent();
        }
        const auto size = spi_request->get_send_count() < Max_frame_size ? spi_request->get_send_count()
                                                                          : static_cast<uint8_t>(Max_frame_size);

        // Decode: command byte, then value MSB first.
        const uint8_t address = (frame[0] >> 1) % Register_count;
        const bool is_write = (frame[0] & 0x01) != 0;
        if (is_write) {
            uint32_t value{};
            for (uint8_t index = 1; index < size; index++) {
                value = value << 8 | frame[index];
            }
            device->the_register[address] = value;
            the_write_count++;
        }

        // Answer with the register value, MSB first, right-aligned in bytes 1..size-1.
        auto *receive = spi_request->get_receive_buffer();
        const auto value = device->the_register[address];
        while (spi_request->is_more_to_receive()) {
            const auto index = spi_request->get_receive_count();
            uint8_t data{};
            if (index > 0) {
                const int shift = 8 * (size - 1 - index);
                data = shift >= 0 && shift < 32 ? static_cast<uint8_t>(value >> shift) : 0;
            }
            if (receive) {
                receive[index] = data;
            }
            spi_request->count_received();
        }

        chip_select.set();
        the_transfer_count++;
        return true;
    }

    Mockup_SPI::Device *Mockup_SPI::find_device(const Abstract::Abstract_IO_pin &chip_select) {
        for (auto &device: the_device) {
            if (device.optional_chip_select == &chip_select) {
                return &device;
            }
        }
        for (auto &device: the_device) {
            if (device.optional_chip_select == nullptr) {
                device.optional_chip_select = &chip_select;
                return &device;
            }
        }
        return nullptr;
    }

    void Mockup_SPI::print_diagnostics() const {
        int devices{};
        for (const auto &device: the_device) {
            devices += device.optional_chip_select != nullptr;
        }
        printf("SPI (mockup): devices=%d, transfers=%u, writes=%u, failures=%u\r\n", devices,
               static_cast<unsigned>(the_transfer_count), static_cast<unsigned>(the_write_count),
               static_cast<unsigned>(the_failure_count));
    }
} // Mockup
