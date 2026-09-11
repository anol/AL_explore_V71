#pragma once

#include "Abstract_ethernet.h"

#include <atomic>
#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>   // must precede windows.h / pcap.h
#include <iphlpapi.h>
#include <pcap.h>

using namespace Abstract;

namespace Windows_platform {
    // Concrete Ethernet_device backend for Windows, built on Npcap (or
    // WinPcap) for raw-frame TX/RX and the IP Helper API for
    // MAC-address/link-status queries. This gives Layer-2 semantics on
    // Windows comparable to Ethernet_samv71_mac's GMAC access -- both
    // hand you and accept complete Ethernet frames -- rather than going
    // through Winsock's IP-level stack.
    //
    // Not compiled/tested in this environment (no Windows SDK or Npcap
    // SDK available here). Build requirements:
    //   - Npcap SDK (https://npcap.com/#download) -- or the older WinPcap
    //     Developer's Pack, which exposes the same API -- on your
    //     include/lib path.
    //   - Link against wpcap.lib, iphlpapi.lib, ws2_32.lib.
    //   - The Npcap (or WinPcap) runtime installed on any machine that
    //     runs the resulting binary.
    //   - The process must run elevated (raw packet capture/injection
    //     needs Administrator privileges on Windows).
    //
    // Threading model: capture happens on a background thread that this
    // class owns internally and drives via pcap_dispatch(); received
    // frames are copied into a small mutex-protected queue. poll() --
    // called from your application thread -- drains that queue and
    // invokes the registered callback from its own calling context,
    // never from the capture thread, matching Ethernet_device's
    // contract. set_receive_callback() and poll() are expected to be
    // called from a single application thread; send_frame() may be
    // called concurrently with poll() from a different thread.
    class Windows_ethernet final : public Abstract_ethernet {
    public:
        struct Adapter_info {
            std::string name;         // pcap device name, e.g. "\Component\NPF_{GUID}"
            std::string description;  // human-readable, e.g. "Intel(R) Ethernet Connection"
        };

        // Enumerates capturable adapters (requires Npcap/WinPcap to be
        // installed). Returns an empty vector and, if errbuf_out is
        // non-null, a diagnostic message on failure.
        [[nodiscard]] static std::vector<Adapter_info> list_adapters(std::string *errbuf_out = nullptr);

        // adapter_name: a pcap device name as returned by
        // list_adapters(), e.g.
        // "\Component\NPF_{4D36E972-E325-11CE-BFC1-08002BE10318}".
        explicit Windows_ethernet(std::string adapter_name) : the_adapter_name(std::move(adapter_name)) {
        }

        ~Windows_ethernet() override { shutdown(); }

        Windows_ethernet(const Windows_ethernet &) = delete;
        Windows_ethernet &operator=(const Windows_ethernet &) = delete;

        // Note: mac_address is informational only -- Windows does not
        // generally let you reprogram a NIC's burned-in MAC address
        // through this API. get_mac_address() returns the adapter's
        // real, OS-reported address (refreshed whenever initialize() or
        // is_link_up() runs).
        Ethernet_status initialize(const uint8_t mac_address[6]) override;
        Ethernet_status shutdown() override;
        [[nodiscard]] bool is_initialized() const override;

        [[nodiscard]] bool is_link_up() const override;
        void get_mac_address(uint8_t mac_address_out[6]) const override;

        [[nodiscard]] Ethernet_status send_frame(const uint8_t *data, size_t length) override;

        Ethernet_status set_receive_callback(Ethernet_frame_callback callback) override;
        void poll() override;

        [[nodiscard]] Ethernet_status get_last_error() const override;

    private:
        static void pcap_dispatch_trampoline(u_char *user, const struct pcap_pkthdr *header,
                                              const u_char *bytes);
        void on_packet_captured(const struct pcap_pkthdr *header, const u_char *bytes);
        void capture_thread_main();

        // Queries GetAdaptersAddresses() for this adapter and refreshes
        // the_mac_address / the_link_up_flag. Const because it only
        // updates mutable "cached live status" members, not object
        // identity.
        bool refresh_mac_and_link_status() const;

        std::string the_adapter_name;
        pcap_t *optional_handle{};
        std::thread the_capture_thread;
        std::atomic<bool> the_capturing_flag{false};

        bool the_initialized_flag{};
        mutable uint8_t the_mac_address[6]{};
        mutable std::atomic<bool> the_link_up_flag{false};
        mutable Ethernet_status the_last_error{Ethernet_status::Ok};

        struct Queued_frame {
            std::vector<uint8_t> data;
        };

        static constexpr size_t Max_queued_frames = 256;
        std::mutex the_queue_mutex;
        std::deque<Queued_frame> the_rx_queue;
        size_t the_dropped_frame_count{};

        // Guards pcap_sendpacket()/the_handle against racing
        // pcap_close() in shutdown() when send_frame() is called from a
        // different thread than poll().
        mutable std::mutex the_send_mutex;

        Ethernet_frame_callback the_receive_callback;
    };
} // Windows_platform
