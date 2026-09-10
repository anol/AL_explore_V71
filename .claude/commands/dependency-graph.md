---
description: Regenerate the module (directory-level) dependency graph and link it from README.md
---

Regenerate `doc/module_dependencies.svg` — the directory-level `#include` dependency
graph — matching the approach already used for it in this repo (see recent conversation
history / git log for the original derivation).

Do **not** also generate a header-file-level (per-.h-file) graph — that was tried and
rejected as too unreadable (88 nodes, extremely wide, unreadable at a glance). Only the
module-level rollup is wanted.

## 1. Determine what CMake actually builds

This project's root `CMakeLists.txt` takes `APP`/`TARGET`/`BOARD`/`PLATFORM` cache
variables and `add_subdirectory()`s a different subset of `src/` depending on them; only
one variant (`V71_hello_world`) is what this graph documents. Before extracting anything:

- Read `CMakePresets.json` at the repo root for the `V71_hello_world` configure preset's
  `cacheVariables` (as of this writing: `APP=Hello_world`, `TARGET=V71_EK_hello_world`,
  `BOARD=V71_EK`, `PLATFORM=SamV71` — re-check, this may have changed).
- Starting at the root `CMakeLists.txt`, substitute those variables into every
  `add_subdirectory(...)` call, then follow each resulting directory's own
  `CMakeLists.txt` recursively for further `add_subdirectory()` calls (e.g. `Board/V71_EK`
  pulls in `Device/IDE3380`; `Dictionary` pulls in `Generated_code`; `Plumbing` pulls in
  `CLI_parser` and `Instruction`; `Platform/SamV71_platform` pulls in `SAMV71Q21B` and
  `Platform/Common_platform`; `Support` pulls in `Console`). The resulting directory set
  is the **only** modules this graph should contain — every other `src/` directory
  (other Targets, `Board/Win11`, the non-`SamV71` `Platform/*` variants, etc.) is real
  code that plainly isn't part of *this* build, and gets excluded entirely, not just its
  edges.
- Within each active module's own `CMakeLists.txt`, note any `target_sources()` entries
  that are commented out (e.g. `#        Sensor_STTS22H.cpp`) — those files exist on disk
  but aren't compiled for any configuration; exclude them individually even though their
  directory is otherwise active. (`*_unit_test.cpp` files sitting directly in an active
  module, e.g. `Utility/`, are usually commented out this way too — check, don't assume.)
- `src_3rd/` (FreeRTOS et al.) is a real `add_subdirectory()` too, but stays out of scope
  the same way it always has — this graph only ever covered `src/`.

## 2. Extract `#include` edges

- Grep every quoted `#include "..."` (ignore commented-out ones, e.g. `// #include "x.h"`)
  across the file set from step 1 (not all of `src/` — only the CMake-active files).
- Resolve each included name to a real project file **by basename**, stripping any path
  prefix in the include string itself first (e.g. `"Generated_code/Foo.h"` or
  `"../Support/Foo.h"` → match on `Foo.h`) — this repo's `#include`s aren't all relative
  to the including file, so path-prefixed matching under-resolves.
- If a basename doesn't match any project file, treat it as external (FreeRTOS/SAM-SDK/
  STM32-HAL/gtest/etc.) and skip it.
- If a basename is ambiguous (matches multiple project files), prefer a match in the same
  directory as the includer; if still ambiguous, flag it rather than guessing.
- Drop true self-includes if any turn up (seen once before, in generated dictionary code
  — `SpectraNode_command_lookup.h` including itself — it's noise, not a real edge).

## 3. Roll up to module level

A module = one source directory under `src/` (e.g. `Application/Hello_world`,
`Platform/SamV71_platform` — leaf directories, not collapsed further up). Every module
from step 1 should appear as a node even if it has no cross-module edges (e.g.
`Platform/SamV71_platform/SAMV71Q21B`, whose one source file only includes SDK headers) —
don't only include modules that happen to show up as an edge endpoint. For every resolved
file-level edge, map both endpoints to their containing directory, dedupe, and drop
same-module edges (a directory including its own sibling file isn't a *module*
dependency).

## 4. Render with Graphviz — match the existing style

- `rankdir=LR`, natural size (no forced `size`/`ratio=fill` — this is a browse/zoom
  reference, not a print deliverable)
- `node [shape=box, style="rounded,filled", fontname="Consolas", fontsize=13]`
- `edge [color="#7a8a80", arrowsize=0.8]`
- Group nodes into `subgraph cluster_N { ... }` blocks by top-level directory
  (`Abstract`, `Application`, `Board`, `Device`, `Dictionary`, `Platform`, `Plumbing`,
  `Repository`, `Support`, `Target`, `Utility`), each cluster `style=dashed,
  color="#c4cdc3"`, with a light `fillcolor` per top-level area (blue-ish for
  Application, green-ish for Platform, copper for Board/Plumbing, red-ish for Target,
  purple for Device, neutral grey for Abstract/Repository/Utility/Dictionary/Support) —
  reuse the exact palette from `doc/module_dependencies.svg`'s current source if you
  still have it, otherwise pick something in that spirit.
- Graph `label` at the bottom (`labelloc=b, fontsize=13`), naming the actual preset used:
  `"Module dependency graph — one node per source directory actually built by CMake\n(preset V71_hello_world: APP=Hello_world, TARGET=V71_EK_hello_world, BOARD=V71_EK, PLATFORM=SamV71)   (arrow: module → module it includes from)"`

Render with `dot -Kdot -Tsvg` (`dot.exe` at `C:\Program Files\Graphviz 2.44.1\bin\`).
Verify zero `foreignObject` elements in the output (`grep -c foreignObject`) — should be
automatic with Graphviz's own text rendering, but it's the check that would have caught
the Mermaid-based diagram that failed to render in this user's markdown previewer earlier
in this project's history.

## 5. Save and embed it

- Overwrite `doc/module_dependencies.svg`.
- Confirm (or add, if missing) an **embedded image** of it — not a plain link; a
  plain-text-only Mermaid SVG once failed to render inline in the user's previewer, but
  that's not a concern with Graphviz's plain-`<text>` output, and the user explicitly
  wants it visible without a click — under the `## Dependencies` section of the
  top-level `README.md`: `![Module dependency graph](doc/module_dependencies.svg)`,
  with a line of context above it.

Leave the rest of that section (and the `## Wiring` section above it) untouched.
