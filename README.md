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

Derived from every `#include "..."` and C++20 `import ...;` in `src/` (SDK/RTOS/third-party
headers excluded), restricted to what CMake actually builds for the `V71_hello_world` preset
(APP=Hello_world, TARGET=V71_EK_hello_world, BOARD=V71_EK, PLATFORM=SamV71) — other targets,
boards, and platform variants are real directories in `src/` but aren't part of this build,
so they're left out. One node per source directory (a "module"), clustered by top-level area
— wide, meant for scrolling/zooming rather than an at-a-glance read. Solid arrows are
`#include` dependencies; dashed arrows are C++20 module imports. Everything under `Type/` is now
a named module — `Type/Abstract` (one module per `Abstract_*` interface), `Type/Basic`
(`Type.Misc_type`, `Type.Status_code`) and `Type/Generic` (`Type.Transfer_request`) — plus
`Domain/IO_pins` and `Support/Console`'s `Support.Console_service`, so every edge into `Type/*`
is dashed. The other `import` edges are the consumers of those two: `Board/V71_EK` and
`Type/Abstract` import `Domain.IO_pins`, `Application/Hello_world` imports both `Domain.IO_pins`
and `Support.Console_service`, and `Platform/Common_platform` imports
`Support.Console_service`. (`Type` and `Support/Plumbing` have no files of their own any more, so neither is a node.)
A module pair with edges in both directions (either kind) is flagged as a two-way dependency:
both arrows render bold and red — there are currently none:

![Module dependency graph](doc/module_dependencies.svg)


