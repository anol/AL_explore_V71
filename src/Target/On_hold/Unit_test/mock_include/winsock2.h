// Minimal stand-in for <winsock2.h>, providing only the symbols
// WindowsEthernet.h/.cpp reference. Not a substitute for the real Windows
// SDK -- see README.md.
#pragma once

using ULONG = unsigned long;
using PCHAR = char*;
using PWCHAR = wchar_t*;
using BYTE = unsigned char;

constexpr int AF_UNSPEC = 0;
