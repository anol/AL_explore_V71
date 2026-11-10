// Host implementation of Platform.Common_stdio: on Windows printf already goes to the console, so there is
// no _write() retargeting to a UART (that lives in Common_platform/Common_stdio.cpp for the embedded targets).

module;
#include <cstdio>

module Platform.Common_stdio;

void Platform::Common_stdio::initialize() const {
    setvbuf(stdout, nullptr, _IONBF, 0);
}
