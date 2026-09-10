# AstraLEO Explore SAMV71 Peripherals, etc.

Using FreeRTOS (FreeRTOSv202604.01-LTS), copied selected code into this project.

## Wiring

Aggregation graphs of the two concrete objects wired together in `main()`
(`V71_EK_hello_world.cpp`):

What `Hello_world` (the application) owns and references:

![Hello_world aggregation graph](doc/hello_world_aggregation.svg)

What `V71_EK_board` (the board) owns and references:

![V71_EK_board aggregation graph](doc/v71_ek_board_aggregation.svg)

Graphviz source (larger/editable version): [`doc/hello_world_wiring_A4.dot`](doc/hello_world_wiring_A4.dot)

## Dependencies

Derived from every `#include "..."` in `src/` (SDK/RTOS/third-party headers excluded),
restricted to what CMake actually builds for the `V71_hello_world` preset (APP=Hello_world,
TARGET=V71_EK_hello_world, BOARD=V71_EK, PLATFORM=SamV71) — other targets, boards, and
platform variants are real directories in `src/` but aren't part of this build, so they're
left out. One node per source directory (a "module"), clustered by top-level area — wide,
meant for scrolling/zooming rather than an at-a-glance read:

![Module dependency graph](doc/module_dependencies.svg)


