//
// Created by aeols on 2026-08-17.
//

#pragma once

#include "Abstract_ethernet.h"

namespace Mockup
{
    using namespace Abstract;

    class Mockup_ethernet final : public Abstract_ethernet
    {
        bool the_initialized_flag{};
        uint8_t the_mac_address[6]{};
        Ethernet_frame_callback the_receive_callback;
        Ethernet_status the_last_error{Ethernet_status::Ok};

    public:
        Ethernet_status initialize(const uint8_t mac_address[6]) override
        {
            for (int i = 0; i < 6; ++i) the_mac_address[i] = mac_address[i];
            the_initialized_flag = true;
            return Ethernet_status::Ok;
        }

        Ethernet_status shutdown() override
        {
            the_initialized_flag = false;
            return Ethernet_status::Ok;
        }

        [[nodiscard]] bool is_initialized() const override { return the_initialized_flag; }
        [[nodiscard]] bool is_link_up() const override { return the_initialized_flag; }

        void get_mac_address(uint8_t mac_address_out[6]) const override
        {
            for (int i = 0; i < 6; ++i) mac_address_out[i] = the_mac_address[i];
        }

        [[nodiscard]] Ethernet_status send_frame(const uint8_t* data, size_t length) override
        {
            if (!the_initialized_flag) return Ethernet_status::Not_initialized;
            if (data == nullptr || length == 0) return Ethernet_status::Invalid_parameter;
            return Ethernet_status::Ok;
        }

        Ethernet_status set_receive_callback(Ethernet_frame_callback callback) override
        {
            the_receive_callback = std::move(callback);
            return Ethernet_status::Ok;
        }

        void poll() override
        {
            if (the_receive_callback)
            {
                uint8_t dummy[4] = {0xDE, 0xAD, 0xBE, 0xEF};
                the_receive_callback(dummy, sizeof(dummy));
            }
        }

        [[nodiscard]] Ethernet_status get_last_error() const override { return the_last_error; }
    };
} // Mockup
