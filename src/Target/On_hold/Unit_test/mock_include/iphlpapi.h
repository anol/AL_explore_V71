// Minimal stand-in for <iphlpapi.h>, providing only the symbols
// WindowsEthernet.h/.cpp reference (field layout confirmed against
// Microsoft's IP_ADAPTER_ADDRESSES_LH reference -- see README.md). Not a
// substitute for the real Windows SDK.
#pragma once
#include "winsock2.h"

constexpr ULONG GAA_FLAG_SKIP_MULTICAST = 0x0002;
constexpr ULONG GAA_FLAG_SKIP_ANYCAST = 0x0004;
constexpr ULONG GAA_FLAG_SKIP_DNS_SERVER = 0x0008;
constexpr ULONG NO_ERROR = 0;
constexpr ULONG ERROR_BUFFER_OVERFLOW = 111;
constexpr int MAX_ADAPTER_ADDRESS_LENGTH = 8;

enum IF_OPER_STATUS { IfOperStatusUp = 1, IfOperStatusDown = 2 };

struct IP_ADAPTER_ADDRESSES {
    IP_ADAPTER_ADDRESSES* Next;
    PCHAR AdapterName;
    BYTE PhysicalAddress[MAX_ADAPTER_ADDRESS_LENGTH];
    ULONG PhysicalAddressLength;
    IF_OPER_STATUS OperStatus;
};

inline ULONG GetAdaptersAddresses(ULONG /*family*/, ULONG /*flags*/, void* /*reserved*/,
                                   IP_ADAPTER_ADDRESSES* /*addresses*/, ULONG* sizePointer) {
    // Mock: reports success with an empty adapter list. Only exists so the
    // compile-verification step can exercise this call site's syntax.
    *sizePointer = 0;
    return NO_ERROR;
}
