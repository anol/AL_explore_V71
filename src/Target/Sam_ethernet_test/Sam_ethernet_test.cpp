// Compile-only sanity check for Abstract_ethernet.h itself: a minimal stub
// implementation exercising every pure virtual + the callback type.

#include "stdio.h"

#include "SamV71_ethernet.h"

int main() {
    const uint8_t mac[6] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x01};

    SamV71_platform::SamV71_ethernet eth{0, 50'000'000};

    if (const auto status = eth.initialize(mac); status == Abstract::Ethernet_status::Already_initialized) {
        printf("Already initialized\n");
    } else if (status != Abstract::Ethernet_status::Ok) {
        printf("Initialize failed, diag=%d\n", static_cast<int>(status));
        return 1;
    }

    size_t received_count = 0;
    eth.set_receive_callback([&received_count](const uint8_t *, size_t length) {
        received_count += length;
    });
    eth.poll();

    uint8_t frame[14] = {0};
    (void) eth.send_frame(frame, sizeof(frame));

    uint8_t read_back[6] = {0};
    eth.get_mac_address(read_back);

    bool mac_matches = true;
    for (int i = 0; i < 6; ++i) {
        if (read_back[i] != mac[i]) mac_matches = false;
    }

    printf("is_initialized=%d is_link_up=%d received_count=%zu mac_matches=%d\n",
           eth.is_initialized(), eth.is_link_up(), received_count, mac_matches);

    eth.shutdown();
    return (mac_matches && received_count == 4) ? 0 : 1;
}
