//
// See Ethernet_windows.h for design notes and build requirements. Not
// compiled/tested in this environment (no Windows SDK / Npcap SDK
// available here).
//

#include "../Windows_ethernet.h"

#include <cstring>

namespace Windows_platform {
    std::vector<Windows_ethernet::Adapter_info> Windows_ethernet::list_adapters(std::string *errbuf_out) {
        std::vector<Adapter_info> result;
        pcap_if_t *devices = nullptr;
        char errbuf[PCAP_ERRBUF_SIZE] = {0};

        if (pcap_findalldevs(&devices, errbuf) != 0) {
            if (errbuf_out != nullptr) {
                *errbuf_out = errbuf;
            }
            return result;
        }

        for (pcap_if_t *d = devices; d != nullptr; d = d->next) {
            Adapter_info info;
            info.name = (d->name != nullptr) ? d->name : "";
            info.description = (d->description != nullptr) ? d->description : "";
            result.push_back(std::move(info));
        }
        pcap_freealldevs(devices);
        return result;
    }

    Ethernet_status Windows_ethernet::initialize(const uint8_t mac_address[6]) {
        if (the_initialized_flag) {
            the_last_error = Ethernet_status::Already_initialized;
            return the_last_error;
        }
        (void) mac_address;  // informational only -- see class comment in the header.

        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        // - snaplen: Max_ethernet_frame_size is enough to capture a
        //   full, untagged Ethernet frame.
        // - promisc=1: also see frames not addressed to this host's MAC.
        //   Set to 0 if you only want unicast-to-us plus
        //   broadcast/multicast.
        // - to_ms=1: a short read timeout keeps pcap_dispatch() in
        //   capture_thread_main() returning often enough to notice
        //   shutdown() promptly.
        optional_handle = pcap_open_live(the_adapter_name.c_str(), static_cast<int>(Max_ethernet_frame_size),
                                          /*promisc=*/1, /*to_ms=*/1, errbuf);
        if (optional_handle == nullptr) {
            the_last_error = Ethernet_status::Hardware_error;
            return the_last_error;
        }

        if (pcap_datalink(optional_handle) != DLT_EN10MB) {
            // Not an Ethernet-framed adapter (e.g. a raw IP or loopback
            // pseudo-adapter) -- this backend only speaks Ethernet
            // frames.
            pcap_close(optional_handle);
            optional_handle = nullptr;
            the_last_error = Ethernet_status::Not_supported;
            return the_last_error;
        }

        refresh_mac_and_link_status();

        the_capturing_flag.store(true, std::memory_order_relaxed);
        the_capture_thread = std::thread(&Windows_ethernet::capture_thread_main, this);

        the_initialized_flag = true;
        the_last_error = Ethernet_status::Ok;
        return Ethernet_status::Ok;
    }

    Ethernet_status Windows_ethernet::shutdown() {
        if (!the_initialized_flag) {
            return Ethernet_status::Ok;
        }

        the_capturing_flag.store(false, std::memory_order_relaxed);
        if (optional_handle != nullptr) {
            pcap_breakloop(optional_handle);  // unblocks a dispatch call if one is waiting on a read
        }
        if (the_capture_thread.joinable()) {
            the_capture_thread.join();
        }

        {
            std::lock_guard<std::mutex> lock(the_send_mutex);
            if (optional_handle != nullptr) {
                pcap_close(optional_handle);
                optional_handle = nullptr;
            }
        }

        the_receive_callback = nullptr;
        the_initialized_flag = false;
        return Ethernet_status::Ok;
    }

    bool Windows_ethernet::is_initialized() const { return the_initialized_flag; }

    bool Windows_ethernet::refresh_mac_and_link_status() const {
        ULONG buffer_len = 15000;  // Microsoft's documented starting size for GetAdaptersAddresses
        std::vector<uint8_t> buffer(buffer_len);
        auto *addresses = reinterpret_cast<IP_ADAPTER_ADDRESSES *>(buffer.data());

        const ULONG flags = GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_DNS_SERVER;
        ULONG result = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, addresses, &buffer_len);
        if (result == ERROR_BUFFER_OVERFLOW) {
            buffer.resize(buffer_len);
            addresses = reinterpret_cast<IP_ADAPTER_ADDRESSES *>(buffer.data());
            result = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, addresses, &buffer_len);
        }
        if (result != NO_ERROR) {
            the_last_error = Ethernet_status::Hardware_error;
            return false;
        }

        // Npcap/WinPcap device names look like "\Device\NPF_{GUID}"; the
        // AdapterName field from GetAdaptersAddresses is that same
        // "{GUID}". Match by substring so this is tolerant of exact
        // prefix differences between Npcap versions/modes.
        for (IP_ADAPTER_ADDRESSES *a = addresses; a != nullptr; a = a->Next) {
            if (a->AdapterName == nullptr) {
                continue;
            }
            if (the_adapter_name.find(a->AdapterName) != std::string::npos) {
                if (a->PhysicalAddressLength == 6) {
                    std::memcpy(the_mac_address, a->PhysicalAddress, 6);
                }
                the_link_up_flag.store(a->OperStatus == IfOperStatusUp, std::memory_order_relaxed);
                return true;
            }
        }

        the_last_error = Ethernet_status::Hardware_error;
        return false;
    }

    bool Windows_ethernet::is_link_up() const {
        if (!the_initialized_flag) {
            return false;
        }
        refresh_mac_and_link_status();
        return the_link_up_flag.load(std::memory_order_relaxed);
    }

    void Windows_ethernet::get_mac_address(uint8_t mac_address_out[6]) const {
        if (mac_address_out == nullptr) {
            return;
        }
        std::memcpy(mac_address_out, the_mac_address, 6);
    }

    Ethernet_status Windows_ethernet::send_frame(const uint8_t *data, size_t length) {
        if (!the_initialized_flag) {
            the_last_error = Ethernet_status::Not_initialized;
            return the_last_error;
        }
        if (data == nullptr || length == 0 || length > Max_ethernet_frame_size) {
            the_last_error = Ethernet_status::Invalid_parameter;
            return the_last_error;
        }

        std::lock_guard<std::mutex> lock(the_send_mutex);
        if (optional_handle == nullptr) {
            the_last_error = Ethernet_status::Not_initialized;
            return the_last_error;
        }
        if (pcap_sendpacket(optional_handle, data, static_cast<int>(length)) != 0) {
            the_last_error = Ethernet_status::Hardware_error;
            return the_last_error;
        }

        the_last_error = Ethernet_status::Ok;
        return Ethernet_status::Ok;
    }

    Ethernet_status Windows_ethernet::set_receive_callback(Ethernet_frame_callback callback) {
        // Only touched from the application thread that also calls
        // poll(); see the threading-model note in the header.
        the_receive_callback = std::move(callback);
        return Ethernet_status::Ok;
    }

    void Windows_ethernet::poll() {
        if (!the_initialized_flag) {
            return;
        }

        std::deque<Queued_frame> drained;
        bool had_drops = false;
        {
            std::lock_guard<std::mutex> lock(the_queue_mutex);
            drained.swap(the_rx_queue);
            if (the_dropped_frame_count > 0) {
                had_drops = true;
                the_dropped_frame_count = 0;
            }
        }
        if (had_drops) {
            // The capture thread's queue hit Max_queued_frames between
            // two poll() calls and had to discard the oldest frame(s) to
            // make room -- poll() isn't being called often enough for
            // the traffic rate. Surfaced via get_last_error() rather
            // than dropped silently.
            the_last_error = Ethernet_status::Hardware_error;
        }

        if (!the_receive_callback) {
            return;
        }
        for (auto &frame: drained) {
            the_receive_callback(frame.data.data(), frame.data.size());
        }
    }

    Ethernet_status Windows_ethernet::get_last_error() const { return the_last_error; }

    void Windows_ethernet::capture_thread_main() {
        while (the_capturing_flag.load(std::memory_order_relaxed)) {
            // A bounded count plus the short read timeout set in
            // initialize() keeps this loop checking the_capturing_flag
            // regularly without needing pcap_breakloop() to win a race
            // against pcap_close().
            pcap_dispatch(optional_handle, /*cnt=*/64, &Windows_ethernet::pcap_dispatch_trampoline,
                          reinterpret_cast<u_char *>(this));
        }
    }

    void Windows_ethernet::pcap_dispatch_trampoline(u_char *user, const struct pcap_pkthdr *header,
                                                      const u_char *bytes) {
        reinterpret_cast<Windows_ethernet *>(user)->on_packet_captured(header, bytes);
    }

    void Windows_ethernet::on_packet_captured(const struct pcap_pkthdr *header, const u_char *bytes) {
        std::lock_guard<std::mutex> lock(the_queue_mutex);
        if (the_rx_queue.size() >= Max_queued_frames) {
            the_rx_queue.pop_front();
            ++the_dropped_frame_count;
        }
        Queued_frame frame;
        frame.data.assign(bytes, bytes + header->caplen);
        the_rx_queue.push_back(std::move(frame));
    }
} // Windows_platform
