// Compile-only sanity check for Abstract_ethernet.h itself: a minimal stub
// implementation exercising every pure virtual + the callback type.

#include <cstdio>

#include "Windows_ethernet.h"

//#include "Mockup/Mockup_ethernet.h"

int main()
{
    // getmac
    // B4-2E-99-FA-ED-F4   \Component\Tcpip_{2C15BB74-10B4-4053-8F36-8D4078F1ADA3}

    char interface_name[]{"\\Component\\NPF_{2C15BB74-10B4-4053-8F36-8D4078F1ADA3}"};
    Windows_platform::Windows_ethernet eth{interface_name};
    const uint8_t mac[6] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x01};

    if (const auto status = eth.initialize(mac); status == Ethernet_status::Already_initialized)
    {
        std::printf("Already initialized\n");
    }
    else if (status != Ethernet_status::Ok)
    {
        std::printf("Initialize failed, diag=%d\n", static_cast<int>(status));
        return 1;
    }

    // size_t received_count = 0;
    // eth.set_receive_callback([&received_count](const uint8_t*, size_t length)
    // {
    //     received_count += length;
    // });
    // eth.poll();
    //
    // uint8_t frame[14] = {0};
    // (void)eth.send_frame(frame, sizeof(frame));

    uint8_t read_back[6] = {0};
    eth.get_mac_address(read_back);
    std::printf("MAC address: %02X.%02X.%02X.%02X.%02X.%02X\n",
                read_back[0], read_back[1], read_back[2], read_back[3], read_back[4], read_back[5]);

    // bool mac_matches = true;
    // for (int i = 0; i < 6; ++i)
    // {
    //     if (read_back[i] != mac[i]) mac_matches = false;
    // }

    // std::printf("is_initialized=%d is_link_up=%d received_count=%zu mac_matches=%d\n",
    //             eth.is_initialized(), eth.is_link_up(), received_count, mac_matches);

    eth.shutdown();
    // return (mac_matches && received_count == 4) ? 0 : 1;
    return 0;
}

