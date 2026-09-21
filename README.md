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
`#include` dependencies; dashed arrows are C++20 module imports. Everything under `Type/`,
`Support/` and `Domain/Generated_code/` is now a named module — `Type/Abstract` (one module per
`Abstract_*` interface), `Type/Basic` (`Type.Misc_type`, `Type.Status_code`), `Type/Generic`
(`Type.Transfer_request`), `Support/Repository` (`Support.Attribute_type` …
`Support.Configuration_repository`), `Support/Plumbing/Instruction` (`Support.Instruction_token`,
`Support.Instruction_major`, `Support.Instruction_lookup`, …), `Support/Plumbing/CLI_parser`
(`Support.CLI_parser`, …), `Support/Console` (`Support.Console_service`) and the generated
dictionary (`Domain.SpectraNode_keyword_lookup`, `Domain.SpectraNode_command_lookup`,
`Domain.SpectraNode_provider_indication`, …) — plus `Domain/IO_pins`. So every edge into
`Type/*`, `Support/*` and `Domain/Generated_code` is dashed, plus the `Domain.IO_pins` imports from
`Application/Hello_world`, `Board/V71_EK` and `Type/Abstract`. The remaining `#include` edges only
point at `Application/*`, `Board/V71_EK`, `Component/*`, `Domain`, `Platform/*` and `Utility`.
(`Type`, `Support` and `Support/Plumbing` have no files of their own any more, so none of them is a node.)
A module pair with edges in both directions (either kind) is flagged as a two-way dependency:
both arrows render bold and red — there are currently none. (The generated dictionary depends on
`Support/Plumbing/Instruction`, never the other way round: `Instruction_major` owns the `Wildcard_id`
constant, which the generated `Keys` enum refers to, and is handed the dictionary's `Literal_value` and
keyword lookup by its callers as arguments.)

![Module dependency graph](doc/module_dependencies.svg)


