# AstraLEO Explore SAMV71 Peripherals, etc.

Build using GCC and C++20.
Using FreeRTOS (FreeRTOSv202604.01-LTS).

## Top-level modules

- **`Application/`** — the app itself: `Hello_world` (composition root, wired up in
  `main()`), `Provider` (the data providers it routes requests to) and `Support` (C helper
  code, e.g. the gamma peak detector and isotope table).
- **`Board/`** — board support packages, one per physical board (currently `V71_EK`),
  wiring together the platform, components and pin table for that board.
- **`Component/`** — drivers for discrete hardware parts (`IDE3380`, `SiPM_bias`,
  `STTS22H`), independent of any one board.
- **`Domain/`** — application-domain types and the generated command/keyword dictionary
  (`Domain/Generated_code`) that ties the console/CLI layer to those types.
- **`Platform/`** — MCU- and RTOS-facing code: `SamV71_platform` (and its `SAMV71Q21B`
  startup/SDK layer), `Common_platform` and `FreeRTOS_platform`.
- **`Support/`** — cross-cutting infrastructure: the console service, the CLI parser and
  instruction dispatch (`Plumbing/`), and the configuration/attribute repository.
- **`Target/`** — top-level build targets that assemble an application, board and platform
  into one firmware image (currently `V71_EK_hello_world`).
- **`Type/`** — shared type definitions: abstract hardware interfaces (`Abstract/`), basic
  value/status types (`Basic/`) and generic transfer-request types (`Generic/`).
- **`Utility/`** — small, dependency-free helpers (ring buffer, simple string/math) used
  throughout the rest of the tree.

![Top-level module dependency graph](doc/top_level_dependencies.svg)

The graph above collapses every source directory into its top-level `src/` area
(`Application/`, `Board/`, `Component/`, `Domain/`, `Platform/`, `Support/`, `Target/`,
`Type/`, `Utility/`). The full, per-source-directory graph it's rolled up from:

## Dependencies

![Module dependency graph](doc/module_dependencies.svg)

## Wiring

Aggregation graphs of the two concrete objects wired together in `main()`
(`V71_EK_hello_world.cpp`):

What `Hello_world` (the application) owns and references:

![Hello_world aggregation graph](doc/hello_world_aggregation.svg)

What `V71_EK_board` (the board) owns and references:

![V71_EK_board aggregation graph](doc/v71_ek_board_aggregation.svg)

Graphviz source (larger/editable version): [`doc/hello_world_wiring_A4.dot`](doc/hello_world_wiring_A4.dot)

## C++20 modules

Several parts of `src/` (`Type/`, `Support/`, `Application/`, `Component/`,
`Board/V71_EK`, the built `Platform/` directories, `Utility/` and
`Domain/Generated_code`) are built as named C++20 modules rather than traditional
header/source pairs. A quick primer:

- **Module interface unit** — declares and exports the module's public API. File
  extension is `.cppm`:
  ```cpp
  export module Support.Console_service;   // module name (dots are just part of the name)

  import Type.Abstract_service;            // pull in another module

  export class Console_service : public Abstract_service { // exported: visible to importers
      ...
  };

  void log_internal(...);                  // not exported: module-private
  ```
- **Module implementation unit** — provides definitions for a module declared elsewhere,
  without re-exporting its interface: `module Support.Console_service;` (no `export`) at
  the top of a `.cpp` file. Counts as depending on whatever module it implements.
- **Importing a module** — `import Some.Module.Name;` (or `export import ...;` to
  re-export it through the importing module) instead of `#include`-ing a header. No
  preprocessor, no include guards, no macro leakage between translation units.
- **Partitions** — a module can be split across files internally with
  `export module Foo:Partition_name;` / `import :Partition_name;`; partitions are only
  visible to other pieces of the same module, never to importers of `Foo`.
- **Naming convention used in this repo** — module names mirror their source directory,
  dotted (e.g. `Support/Console/Console_service.cppm` exports `Support.Console_service`),
  which is why the dependency graphs above draw `import` edges (dashed) between the same
  directory nodes as `#include` edges (solid).

This project still mixes plain headers (mainly SDK/RTOS/third-party code, plus a few
C-API sources) with modules, hence the graphs above track both `#include` and `import`
edges rather than assuming one or the other.
