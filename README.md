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
`Support/`, `Application/`, `Component/`, `Board/V71_EK`, the built `Platform/` directories, `Utility/` and
`Domain/Generated_code/` is now a named module — `Type/Abstract` (one module per
`Abstract_*` interface), `Type/Basic` (`Type.Misc_type`, `Type.Status_code`), `Type/Generic`
(`Type.Transfer_request`), `Support/Repository` (`Support.Attribute_type` …
`Support.Configuration_repository`), `Support/Plumbing/Instruction` (`Support.Instruction_token`,
`Support.Instruction_major`, `Support.Instruction_lookup`, …), `Support/Plumbing/CLI_parser`
(`Support.CLI_parser`, …), `Support/Console` (`Support.Console_service`) and the generated
dictionary (`Domain.SpectraNode_keyword_lookup`, `Domain.SpectraNode_command_lookup`,
`Domain.SpectraNode_provider_indication`, …), the application (`Application.Hello_world`,
`Application.Request_router`, the five `Application.*_provider` modules, `Application.Cadence_control`),
the components (`Component.IDE3380_interface`, `Component.Histogram_storage`, `Component.Bias_calibration`, …),
the board (`Board.V71_EK_board`, `Board.V71_EK_pin_table`) and the platform (`Platform.SamV71_clock`,
`Platform.SamV71_USART1`, `Platform.FreeRTOS_queue`, `Platform.Diagnostic`, …) and the utilities
(`Utility.Ringbuffer`, `Utility.Simple_string`, `Utility.Simple_math`, …) — plus `Domain/IO_pins`. (The C sources in `Application/Support` — `gamma_peak_detector`, `Isotope_table` —
stay plain headers, since they are a C API, and so does `Component/STTS22H`, whose header is entirely
commented out; `Platform/SamV71_platform/SAMV71Q21B` is startup code and the FreeRTOS port.) So every edge into
`Type/*`, `Support/*`, `Application/*`, `Component/*` (except `STTS22H`), `Board/V71_EK`, `Platform/*`,
`Utility` and `Domain/Generated_code` is dashed (`Domain.IO_pins` itself imports
`Type.Abstract_IO_pin` for the pin id type), plus the `Domain.IO_pins` imports from
`Application/Hello_world` and `Board/V71_EK`. The remaining `#include` edges only
point at `Component/STTS22H` and `Domain` (`Persistent_parameter_id.h`, `Dictionary.h`). A module implementation unit (`module X;`) counts as
an import of `X`, so `Platform/SamV71_platform` → `Platform/Common_platform` is drawn for `Diagnostic.cpp`.
(`Type`, `Support` and `Support/Plumbing` have no files of their own any more, so none of them is a node.)
A module pair with edges in both directions (either kind) is flagged as a two-way dependency:
both arrows render bold and red — there are currently none. (The generated dictionary depends on
`Support/Plumbing/Instruction`, never the other way round: `Instruction_major` owns the `Wildcard_id`
constant, which the generated `Keys` enum refers to, and is handed the dictionary's `Literal_value` and
keyword lookup by its callers as arguments.)

![Module dependency graph](doc/module_dependencies.svg)


