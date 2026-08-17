// Minimal stand-in for <pcap.h> (Npcap/WinPcap/libpcap), providing only
// the symbols WindowsEthernet.h/.cpp reference, matching the long-stable
// public pcap API surface. Not a substitute for the real Npcap SDK -- see
// README.md.
#pragma once
#include "winsock2.h"

using u_char = unsigned char;

constexpr int PCAP_ERRBUF_SIZE = 256;
constexpr int DLT_EN10MB = 1;

struct pcap;
using pcap_t = pcap;

struct pcap_pkthdr {
    long tv_sec;
    long tv_usec;
    unsigned int caplen;
    unsigned int len;
};

struct pcap_if_t {
    pcap_if_t* next;
    char* name;
    char* description;
};

using pcap_handler = void (*)(u_char* user, const pcap_pkthdr* header, const u_char* bytes);

inline pcap_t* pcap_open_live(const char*, int, int, int, char*) { return nullptr; }
inline void pcap_close(pcap_t*) {}
inline int pcap_datalink(pcap_t*) { return DLT_EN10MB; }
inline int pcap_dispatch(pcap_t*, int, pcap_handler, u_char*) { return 0; }
inline int pcap_sendpacket(pcap_t*, const u_char*, int) { return 0; }
inline void pcap_breakloop(pcap_t*) {}
inline int pcap_findalldevs(pcap_if_t**, char*) { return 0; }
inline void pcap_freealldevs(pcap_if_t*) {}
