# C++ / CMake coding style guide (v3)

Sources for this guide, in order of authority:

1. `Abstract_target.h` — the original hand-written reference file (class
   layout, member-naming prefixes, constructor shape, `[[nodiscard]]`
   usage, formatting).
2. Explicit corrections given after v1 (compile-time constants, `static`
   data members, and enum values are capitalized with no prefix).
3. The project's own refactor of the generated code — `Ethernet_unit_test`
   (`src/Abstract`, `src/Platform/*_platform`, `src/Unit_test`, and the
   accompanying CMake files) — which introduced the module/directory
   structure, the `Abstract_`/`<Platform>_` class-naming split, the
   namespace-per-module convention, and all CMake conventions below.

Each rule below is tagged with which source it came from:

- **Observed (reference file)** — appears directly in `Abstract_target.h`.
- **Observed (project)** — appears directly in the project's own
  refactored code; this is now the primary source for anything the single
  reference file couldn't show (directories, CMake, multi-module
  namespacing).
- **Specified** — settled by an explicit instruction rather than found in
  either source.
- **Extended** — a generalization to a situation neither source covers
  yet. Flagged so it can be overridden if it doesn't match intent.

> **Migration note (updated):** `Unit_test.cpp` and `Windows_platform` are
> now fully migrated and internally consistent -- both are the reference
> for the rules below. `src/Platform/SamV71_platform` (`SamV71_ethernet.h`/`.cpp`)
> and the root `CMakeLists.txt` are **still unmigrated**: the SAMV71 files
> still reference the pre-refactor names (`Ethernet_device.h`,
> `Ethernet::Ethernet_samv71_mac`) instead of `Abstract_ethernet.h`/
> `SamV71_ethernet`, and the root `CMakeLists.txt` still only
> `add_subdirectory()`s `Windows_platform`, not `SamV71_platform`. Treat
> those two as not yet caught up, not as evidence of an alternate
> convention.

## Naming

| Element | Rule | Example | Basis |
|---|---|---|---|
| Abstract/interface class | `Abstract_<domain>` | `Abstract_ethernet` | Observed (project) — directly reuses the `Abstract_` prefix from the reference file's own class name, rather than repeating an enclosing namespace name |
| Concrete/platform class | `<Platform>_<domain>` | `SamV71_ethernet`, `Windows_target` | Observed (project) — prefixed with the platform name instead of `Abstract_` or the shared namespace name, so the class name alone tells you whether it's the cross-platform contract or one platform's implementation |
| Namespace (shared/common layer) | Domain word, independent of directory name | `namespace Ethernet` (in `src/Abstract/`) | Observed (project) |
| Namespace (per-platform layer) | Matches its platform directory exactly | `namespace SamV71_platform`, `namespace Windows_platform` | Observed (project) |
| Method name | `snake_case`, verb-first for actions, `is_`/`get_` prefix for queries | `initialize()`, `get_mac_address()`, `is_link_up()` | Observed (reference file) |
| Reference member (injected collaborator) | `use_<name>` | `use_application`, `use_board` | Observed (reference file) |
| Owned value/state member | `the_<name>` | `the_mac_address`, `the_last_error` | Observed (reference file) |
| Owned boolean flag member | `the_<name>_flag` | `the_initialized_flag`, `the_capturing_flag` | Observed (reference file, generalized) |
| Nullable/optional pointer member | `optional_<name>` | `optional_handle` | Observed (reference file) |
| Constructor parameter | Bare noun, no prefix | `Windows_ethernet(std::string adapter_name)` | Observed (reference file) |
| Compile-time constant | Capitalized, no prefix | `Max_ethernet_frame_size`, `Num_rx_descriptors` | Specified |
| `static` data member | Capitalized, no prefix | `Instance` | Specified |
| Enum value | Capitalized, no prefix | `Ethernet_status::Ok`, `::Hardware_error` | Specified |
| Type alias / `using` | Same `Capitalized_snake_case` shape as classes | `Ethernet_frame_callback` | Extended |
| Enum class name | Same `Capitalized_snake_case` shape as classes | `Ethernet_status` | Extended |
| Directory / module name | Same `Capitalized_snake_case` shape as classes, one module per directory | `Abstract`, `Platform`, `SamV71_platform`, `Windows_platform`, `Unit_test` | Observed (project) |
| File name | Matches its class name exactly | `Windows_ethernet.h` defines `class Windows_ethernet` | Observed (reference file + project) |

## Module / directory structure

```
src/
  Abstract/                    shared, platform-independent interface + common types
    Abstract_ethernet.h          (namespace Ethernet)
    CMakeLists.txt
  Platform/                    grouping directory only, no code directly inside
    SamV71_platform/             one directory per concrete backend
      SamV71_ethernet.h          (namespace SamV71_platform)
      SamV71_ethernet.cpp
      CMakeLists.txt
    Windows_platform/
      Windows_ethernet.h         (namespace Windows_platform)
      Windows_ethernet.cpp
      CMakeLists.txt
  Unit_test/                   test harness + mock headers for host-side compilation
    Unit_test.cpp
    mock_include/
    CMakeLists.txt
CMakeLists.txt                 root: wires the modules together
CMakePresets.json
```

Rules:

- One directory per module; one (or one header + one source) file pair
  per class, filename == class name.
- The shared/abstract layer sits in its own directory (`Abstract`) at the
  same level as `Platform`, not nested under it — it's depended on by
  every platform module, not owned by any one of them.
- Each concrete platform gets its own subdirectory under `Platform/`,
  named `<Platform>_platform`, containing exactly the files for that
  platform's `<Platform>_ethernet` class plus its own `CMakeLists.txt`.
- Cross-module `#include`s reference the target header path-relative to
  `src/`, as `<ModuleDir>/<Header>.h` -- e.g.
  `#include "Abstract/Abstract_ethernet.h"` -- resolved through the
  accumulated include path rather than an explicit `../..` relative path
  from the including file's own location. This works because each
  library-style module's `CMakeLists.txt` puts its own parent directory
  on the shared target's include path (`Abstract/CMakeLists.txt` adds
  `src/`, so `Abstract/Abstract_ethernet.h` resolves from anywhere in the
  build -- see "CMake conventions" below). Settled by `Windows_ethernet.h`
  and `Unit_test.cpp`, both of which now use this form; apply it to
  `SamV71_platform` once that module is brought up to date.
- A platform-implementation header pulls the shared layer's names into
  scope with a single file-scope `using namespace Ethernet;`, placed
  after the `#include`s and before the platform's own `namespace { ... }`
  block (see `Windows_ethernet.h`). This lets the class body refer to
  `Abstract_ethernet`, `Ethernet_status`, `Ethernet_frame_callback`, etc.
  unqualified. Worth knowing the trade-off if this expands: a
  using-directive at file scope in a header applies to every file that
  includes it, so it's a deliberate convenience-over-isolation choice for
  this project, not a default to reach for casually in new headers.

*(Observed (project) throughout this section.)*

## CMake conventions

*(All Observed (project) — this is new territory the reference file
couldn't cover.)*

- Root `CMakeLists.txt`:
  ```cmake
  cmake_minimum_required(VERSION 4.3)
  set(CMAKE_CXX_STANDARD 20)

  project("Ethernet_unit_test")

  set(THE_TARGET_NAME ${PROJECT_NAME})
  set(SOURCE_ROOT_DIR ${CMAKE_SOURCE_DIR}/src)

  add_executable(${THE_TARGET_NAME}
  )

  add_subdirectory(${SOURCE_ROOT_DIR}/Unit_test ${CMAKE_BINARY_DIR}/Unit_test)
  add_subdirectory(${SOURCE_ROOT_DIR}/Abstract ${CMAKE_BINARY_DIR}/Abstract)
  add_subdirectory(${SOURCE_ROOT_DIR}/Platform/Windows_platform ${CMAKE_BINARY_DIR}/Platform)
  ```
  `project(...)` name is double-quoted. `add_executable()` takes no
  direct sources -- every source comes in through `add_subdirectory()`,
  one call per module, each given an explicit binary subdirectory.
- CMake script variables: `ALL_CAPS_SNAKE_CASE`, with a `THE_` prefix on
  the project's own singleton values (`THE_TARGET_NAME`) -- the CMake
  analogue of the C++ `the_` member prefix -- and no prefix on plain
  path/config variables (`SOURCE_ROOT_DIR`).
- Every module directory has its own `CMakeLists.txt`, all targeting the
  same shared `${THE_TARGET_NAME}` executable (there's only one target in
  the whole build; every module's `CMakeLists.txt` just adds to it).
  `target_include_directories` differs by the module's role:
  - A **library-style module** (something other modules include from --
    `Abstract`, `SamV71_platform`, `Windows_platform`) exposes its own
    *parent* directory, so siblings can address it as
    `<ThisModuleDir>/<Header>.h`:
    ```cmake
    target_include_directories(${THE_TARGET_NAME} PUBLIC
            ..
    )
    target_sources(${THE_TARGET_NAME} PUBLIC
            Windows_ethernet.cpp
            Windows_ethernet.h
    )
    ```
  - A **leaf/consumer module** (nothing includes from it -- `Unit_test`)
    instead adds only what its own sources need: itself (`.`) plus any
    private support directories (`mock_include`):
    ```cmake
    target_include_directories(${THE_TARGET_NAME} PUBLIC
            .
            mock_include
    )
    target_sources(${THE_TARGET_NAME} PUBLIC
            Unit_test.cpp
            mock_include/iphlpapi.h
            mock_include/pcap.h
            mock_include/sam.h
            mock_include/winsock2.h
    )
    ```
  8-space continuation indent under both calls in every case; list the
  module's own `.cpp` before its `.h`, then any remaining files
  alphabetized.
- `CMakePresets.json`: one `hidden` `base` preset (Ninja generator,
  `binaryDir` under `build/${presetName}`, `CMAKE_EXPORT_COMPILE_COMMANDS`
  on), then one preset per platform target inheriting it. Each preset is
  named identically to the concrete class it builds (`Windows_target`,
  `SamV71_ethernet`) and sets plain-word `ALL_CAPS` cache variables
  (`TARGET`, `PLATFORM`, and platform-specific extras like
  `GNU_VERSION`) to select that platform's module at configure time.
  Matching `buildPresets` entries reference each `configurePreset` by the
  same name. The file is strict JSON -- no trailing commas after a
  preset's last `cacheVariables` entry.

## File structure (per source file)

1. `#pragma once`.
2. One blank line.
3. Standard-library includes (angle brackets), alphabetized.
4. Blank line, then project includes (quotes, explicit relative paths --
   see "Module / directory structure" above).
5. Everything else — including the entire class — lives inside a
   namespace block: `namespace Name { ... } // Name`. The closing-brace
   comment is the bare namespace name, not `// namespace Name`.

*Not reproduced:* the reference file's `// Created by <user> on
<date>.` header banner is an IDE-generated (CLion) file-creation stamp
tied to real authorship metadata; not reproduced in Claude-authored
files since fabricating an author/date would be inaccurate.

## Class layout

1. Private section first, **no explicit `private:` label** — relies on
   the class default. This holds all data members and any private
   helper methods.
2. `public:` labeled explicitly, containing (in order): constructor(s),
   destructor, deleted copy/move operations (if any), then the rest of
   the public API in roughly the order a caller would use them
   (lifecycle → queries → actions).
3. Constructors use a member-initializer list, one line for the
   signature, `:` starting the next line, all initializers on that one
   line, opening `{` at the end:
   ```cpp
   explicit Windows_ethernet(std::string adapter_name)
       : the_adapter_name(std::move(adapter_name)) {
   }
   ```
   **No stray trailing `;` after the closing `}`** of a function body.
4. `~ClassName() override { shutdown(); }` (or `= default;` for the
   abstract base) immediately follows the constructor(s).
5. Default member initializers use brace-init (`{}`), including for raw
   pointers (`pcap_t *optional_handle{};` rather than `= nullptr`).

## Function style

- `virtual` is written explicitly on every virtual method in the
  abstract base, even where it's also marked `= 0`.
- Mark a method `const` whenever it doesn't mutate observable state.
- Short bodies collapse to a single line; longer bodies use normal
  multi-line braces.
- `[[nodiscard]]` goes on pure accessor/getter methods and on any other
  method whose return value would silently hide a bug if ignored --
  status/bool-returning queries and fallible actions (`is_link_up()`,
  `get_last_error()`, `send_frame()`).
- Reference/pointer declarator symbols (`&`, `*`) attach to the
  identifier, not the type: `Type &name`, `Type *name`.

## Formatting

- 4-space indentation in C++; CMake call bodies use 8-space continuation
  indent under the opening line.
- Opening braces stay on the same line as the namespace/class/function
  signature (Stroustrup/K&R-ish, not Allman).
- One blank line between logically distinct members/sections; no blank
  line required between tightly related one-liners.

## Applying this guide

When writing new C++ or CMake for this project (or asking Claude to),
point at this file. It supersedes v1's flatter `include/`/`src/` layout
and `Ethernet_`-prefixed class names -- new code should follow the
module/directory structure, `Abstract_`/`<Platform>_` class naming, and
CMake conventions above. v3 adds the `using namespace Ethernet;` header
convention, the CMake include-directory pattern, and settles cross-module
`#include`s on the path-relative-to-`src/` form, now that
`Windows_platform`/`Unit_test` are fully migrated. The unmigrated
`SamV71_platform`/root `CMakeLists.txt` are called out above rather than
folded in as settled rules -- once that module is brought up to date
(including switching its own `#include "Ethernet_device.h"` to
`#include "Abstract/Abstract_ethernet.h"`), this guide should be
re-checked once more.
