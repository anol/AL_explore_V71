# Ethernet abstraction: Ethernet_device + SAMV71 GMAC + Windows (Npcap)

A small, dependency-light Layer-2 Ethernet abstraction with two concrete
backends: an on-chip driver for the SAMV71's GMAC peripheral, and a
Windows backend built on Npcap/WinPcap. Both hand you and accept raw
Ethernet frames (destination MAC, source MAC, EtherType, payload -- no
FCS, which each backend's hardware/driver generates or strips for you).

Code style follows `STYLE_GUIDE.md` (extracted from a reference file
provided for this project) -- see that file for the naming/formatting
rules and to reuse them in future work.

## Layout

```
include/
  Ethernet_device.h       abstract interface (platform-independent)
  Ethernet_samv71_mac.h   SAMV71 GMAC concrete backend
  Ethernet_windows.h      Windows/Npcap concrete backend
src/
  Ethernet_samv71_mac.cpp
  Ethernet_windows.cpp
verify/                   compile-only sanity check (see "Verification" below)
STYLE_GUIDE.md             the coding style this project follows
```

All types live in `namespace Ethernet`.

## Design

* **Layer-2, not sockets.** `send_frame()`/the receive callback deal in
  complete Ethernet frames, matching what a MAC peripheral actually does.
  There's no IP stack here.
* **Status codes, not exceptions.** Every fallible call returns an
  `Ethernet_status`. This keeps the same interface usable on an
  exceptions-disabled Cortex-M build and a normal desktop build without
  the two backends diverging in error-handling style.
* **Event-driven receive, dispatched outside interrupt/capture context.**
  You register a callback with `set_receive_callback()` and call `poll()`
  regularly (once per main-loop iteration, once per RTOS task tick, or
  from a dedicated thread). `poll()` is what actually invokes your
  callback -- always from the calling context of `poll()` itself, never
  from the SAMV71's GMAC ISR and never from Windows' background capture
  thread. That keeps the callback contract simple: no ISR-safety or
  capture-thread-safety rules for your handler to worry about, on either
  platform.

## Usage

```cpp
#include "Ethernet_device.h"
#include "Ethernet_samv71_mac.h"   // or Ethernet_windows.h

void on_frame(const uint8_t *data, size_t length) {
    // ... handle one received Ethernet frame ...
}

int main() {
    static Ethernet::Ethernet_samv71_mac eth(/*phy_address=*/0, /*mck_hz=*/150000000);
    const uint8_t mac[6] = {0x02, 0x00, 0x00, 0x12, 0x34, 0x56};

    if (eth.initialize(mac) != Ethernet::Ethernet_status::Ok) {
        // handle error via eth.get_last_error()
    }
    eth.set_receive_callback(on_frame);

    while (true) {
        eth.poll();          // dispatches any frames that arrived
        // ... rest of your main loop ...
    }
}
```

```cpp
#include "Ethernet_windows.h"

int main() {
    auto adapters = Ethernet::Ethernet_windows::list_adapters();
    // pick adapters[i].name, typically by matching .description against
    // what the user selected, or by taking the first non-loopback entry.

    Ethernet::Ethernet_windows eth(adapters[0].name);
    const uint8_t mac[6] = {0};  // informational only on this backend
    (void) eth.initialize(mac);
    eth.set_receive_callback(on_frame);

    while (running) {
        eth.poll();
        Sleep(1);
    }
}
```

## Building

### SAMV71 (`Ethernet_samv71_mac.h/.cpp`)

Requires the Microchip CMSIS-SAMV71 device pack (provides `sam.h` for your
exact part, e.g. via `-D__SAMV71Q21B__`) and a bare-metal/RTOS project that
already brings up the core clock tree (MCK) before calling `initialize()`,
since it needs to know MCK's frequency to program a compliant MDIO clock
divider.

This driver was **not compiled or hardware-tested** in the environment
that produced it -- there's no SAMV71 toolchain available there. Register
field names and bit positions were grounded against:

- Microchip's online GMAC documentation ([Receive Buffers](https://onlinedocs.microchip.com/oxy/GUID-5474CCCD-F385-4AD7-9759-539BB1019357-en-US-10/GUID-7598067A-554B-41C9-851C-DDDC37B436BA.html), [Transmit Buffer List](https://onlinedocs.microchip.com/oxy/GUID-05624835-FA79-4FB8-AB53-EB1F2D7BCC20-en-US-7/GUID-2F96967E-3391-4D09-A225-E77C684E0235.html))
- Microchip/Atmel's open-source ASF CMSIS header ([`component/gmac.h`](https://github.com/avrxml/asf/blob/master/sam/utils/cmsis/same70/include/component/gmac.h))
- Oryx Embedded's CycloneTCP SAMV71 reference driver ([`samv71_eth_driver.c`](https://www.oryx-embedded.com/doc/samv71__eth__driver_8c_source.html))

Before shipping this on real hardware:

1. Build against your actual CMSIS-SAMV71 pack and resolve any
   macro-name differences between pack versions.
2. Double-check the MDC clock-divider thresholds in
   `select_mdc_clock_divider()` against your exact datasheet revision.
3. Confirm your board's PHY address and that its Basic Mode Status
   Register (address `0x01`) exposes the standard IEEE 802.3 link-status
   bit; some PHYs need a vendor-specific register instead.
4. Test cache maintenance under real DMA traffic -- see the
   cache-line-sharing caveat documented next to the descriptor arrays in
   `Ethernet_samv71_mac.h`.
5. If you need more than one active RX/TX queue, wire up GMAC's priority
   queues 1-4 (each needs a "permanently used" dummy descriptor per your
   part's errata) -- this driver only uses queue 0.

### Windows (`Ethernet_windows.h/.cpp`)

Requires the [Npcap SDK](https://npcap.com/#download) (or the older
WinPcap Developer's Pack, which exposes the same API) on your include/lib
path, and the Npcap/WinPcap *runtime* installed on any machine that runs
the built binary. Link against `wpcap.lib`, `iphlpapi.lib`, `ws2_32.lib`.

The process must run elevated -- raw packet capture/injection requires
Administrator privileges on Windows.

Also not compiled/tested in this environment (no Windows SDK or Npcap SDK
available there); API usage (`pcap_open_live`, `pcap_dispatch`,
`pcap_sendpacket`, `GetAdaptersAddresses`) was written against the
documented, long-stable Win32/Npcap surfaces, with the exact
`IP_ADAPTER_ADDRESSES` field layout confirmed against [Microsoft's
reference](https://learn.microsoft.com/en-us/windows/win32/api/iptypes/ns-iptypes-ip_adapter_addresses_lh).

## Verification

`src/verify/` contains a compile-only sanity check: a stub `Ethernet_device`
implementation compiled against the real `Ethernet_device.h`, plus the
two concrete backends compiled against small mock stand-ins for `sam.h`
and `pcap.h`/`iphlpapi.h` (reproducing just the struct fields, macros,
and function signatures this code calls). This catches C++-level
mistakes (typos, signature drift between a `.h` declaration and its
`.cpp` definition, override mismatches) but, since the mocks aren't the
real device headers, it cannot confirm hardware register semantics or
actual Npcap behavior -- that still requires building against the real
SDKs listed above.

All three components compile cleanly with `g++ -std=c++17 -Wall -Wextra`
against their respective (real or mock) headers, with zero warnings.
