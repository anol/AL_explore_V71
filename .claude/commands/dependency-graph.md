---
description: Regenerate the module (directory-level) dependency graph and link it from README.md
---

Regenerate `doc/module_dependencies.svg` — the directory-level dependency graph, covering
both `#include` dependencies and C++20 module `import` dependencies — matching the approach
already used for it in this repo (see recent conversation history / git log for the
original derivation).

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
  pulls in `Device/IDE3380`; `Dictionary` pulls in `Generated_code`; `Support/Plumbing`
  pulls in `CLI_parser` and `Instruction`; `Platform/SamV71_platform` pulls in
  `SAMV71Q21B` and `Platform/Common_platform`; `Support` pulls in `Console`; `Type` pulls
  in `Abstract`). The resulting directory set is the **only** modules this graph should
  contain — every other `src/` directory (other Targets, `Board/Win11`, the non-`SamV71`
  `Platform/*` variants, etc.) is real code that plainly isn't part of *this* build, and
  gets excluded entirely, not just its edges. Re-check the directory layout itself each
  time, not just the cache variables — `Plumbing` and `Repository` used to be top-level
  `src/` directories and moved under `src/Support/` in September 2026, and `Type/Abstract`
  used to be a bare top-level `src/Abstract/`; a stale mental model of the tree will
  misplace clusters (see step 4) even when the extracted edges are otherwise correct.
- Within each active module's own `CMakeLists.txt`, note any `target_sources()` entries
  that are commented out (e.g. `#        Sensor_STTS22H.cpp`) — those files exist on disk
  but aren't compiled for any configuration; exclude them individually even though their
  directory is otherwise active. (`*_unit_test.cpp` files sitting directly in an active
  module, e.g. `Utility/`, are usually commented out this way too — check, don't assume.)
  A directory that ends up with zero active files of its own (e.g. `Support`, which only
  `add_subdirectory()`s `Console` and lists no `target_sources()`) is a pure aggregator,
  not a module — give it no node, matching how modules with real files always get one.
- A header physically present in an active module's directory but **not** listed in
  `target_sources()` at all (not even commented out) is still real and still reachable via
  `#include` from a listed file in the same directory — CMake's `target_sources()` doesn't
  gate whether a header can be included, only whether it's a compiled translation unit or
  shown in an IDE. Don't silently drop such headers' own `#include`/`import` lines just
  because they're unlisted (seen twice: `Application/Hello_world/Mockup_persistent_storage.h`
  and `Type/Abstract/Abstract_service.h`); do still fully exclude files that are explicitly
  commented out in `target_sources()`, per the rule above.
- `src_3rd/` (FreeRTOS et al.) is a real `add_subdirectory()` too, but stays out of scope
  the same way it always has — this graph only ever covered `src/`.

## 2. Extract edges

Two different kinds of edge exist in this codebase and both belong in the graph, styled
differently (see step 4) — don't collapse them into one:

**`#include` edges:**
- Grep every `#include "..."` **and** `#include <...>` line (ignore commented-out ones,
  e.g. `// #include "x.h"`) across the file set from step 1 (not all of `src/` — only the
  CMake-active files). Don't grep quoted includes only — this repo mixes both forms for
  its own project headers (e.g. `Support/Plumbing/Instruction/Instruction_lookup.h` uses
  `#include <Diagnostic.h>` and `#include <Instruction/Instruction_major.h>`), because
  `target_include_directories()` puts every module's own directory on the include path
  either way; only skip an angle-bracket include once you've confirmed by basename that
  it's a real external header (stdlib, FreeRTOS, SAM-SDK, gtest, etc.), not because of
  which bracket style it uses.
- Resolve each included name to a real project file **by basename**, stripping any path
  prefix in the include string itself first (e.g. `"Generated_code/Foo.h"` or
  `"../Support/Foo.h"` → match on `Foo.h`) — this repo's `#include`s aren't all relative
  to the including file, so path-prefixed matching under-resolves.
- If a basename doesn't match any project file, treat it as external (FreeRTOS/SAM-SDK/
  STM32-HAL/gtest/etc.) and skip it. This also covers stale/dead includes that don't
  resolve to anything in the repo at all (e.g. `Type/Abstract/Abstract_timer.h` including
  a nonexistent `Utility/Utility_types.h`) — treat those the same as external, don't guess
  a fix.
- If a basename is ambiguous (matches multiple project files), prefer a match in the same
  directory as the includer; if still ambiguous, flag it rather than guessing.
- Drop true self-includes if any turn up (seen once before, in generated dictionary code
  — `SpectraNode_command_lookup.h` including itself — it's noise, not a real edge).

**C++20 module `import` edges** (this project has started using named modules, e.g.
`src/Support/Console/*.cppm` exporting `Support.Console_service`):
- Grep every `import <name>;` / `export import <name>;` line across the same active file
  set (headers and `.cpp` files can `import` a module just as readily as a `.cppm` can).
- A partition import (`import :Partition_name;`, or the `module Foo:Partition_name;`
  declaration a partition file opens with) stays inside the module that owns it — it never
  produces a cross-module edge, and in practice it's always same-directory too, so it gets
  dropped as a same-module edge in step 3 regardless.
- A non-partition `import Some.Module.Name;` names an **exported** module (`export module
  Some.Module.Name;`, found via the same grep) — locate which active file exports it, map
  that file to its containing directory, and that's the edge target; the importing file's
  directory is the source.
- A module **implementation unit** (`module Some.Module.Name;`, without `export`) depends on
  the module it implements, so it counts as an import of `Some.Module.Name`: the edge runs from
  the implementation file's directory to the directory of the file that has the `export module`.
  That matters when the two live in different directories (e.g. `Platform/SamV71_platform/
  Diagnostic.cpp` implements `Platform.Diagnostic`, whose interface is in `Platform/Common_platform`);
  the same-directory case drops out in step 3.

## 3. Roll up to module level

A module = one source directory under `src/` (e.g. `Application/Hello_world`,
`Platform/SamV71_platform` — leaf directories, not collapsed further up). Every module
from step 1 should appear as a node even if it has no cross-module edges (e.g.
`Platform/SamV71_platform/SAMV71Q21B`, whose one source file only includes SDK headers) —
don't only include modules that happen to show up as an edge endpoint. For every resolved
file-level edge (`#include` or `import`), map both endpoints to their containing directory,
dedupe, and drop same-module edges (a directory including or importing its own sibling file
isn't a *module* dependency) — keep the two kinds separate through this step, since a given
module pair can legitimately have one, the other, or (in principle) both.

After dedupe, check every module pair for a **two-way (circular) dependency**: both `A → B`
and `B → A` present in the combined edge set, regardless of which edge kind produced either
direction (an `#include` one way and an `import` the other still counts). Flag every such
pair — this is a real architectural smell worth surfacing, not noise. As of this writing
there are 8 flagged pairs: `Application/Hello_world ↔ Application/Provider`,
`Application/Hello_world ↔ Application/Support`, `Application/Hello_world ↔
Component/IDE3380`, `Application/Hello_world ↔ Component/SiPM_bias`,
`Application/Hello_world ↔ Support/Console`, `Application/Support ↔ Component/IDE3380`,
`Board/V71_EK ↔ Platform/SamV71_platform`, and `Domain ↔ Domain/Generated_code`.

## 4. Render with Graphviz — match the existing style

- `rankdir=LR`, natural size (no forced `size`/`ratio=fill` — this is a browse/zoom
  reference, not a print deliverable)
- `node [shape=box, style="rounded,filled", fontname="Consolas", fontsize=13]`
- `#include` edges: `edge [color="#7a8a80", arrowsize=0.8]` (the default edge style).
  `import` edges: same arrowsize but `style=dashed, color="#3a6ea5", penwidth=1.3` — set
  per-edge, not as a second global `edge` default, since both kinds coexist in one graph.
- Both edges of a **two-way module pair** (flagged in step 3) override the above: draw
  *both* directions solid, `color="#c0392b", penwidth=2.2` (bold red), regardless of which
  edge kind(s) produced them — the point is to flag the module pair as mutually dependent,
  so the red/bold treatment takes priority over the normal solid/dashed distinction for
  just those edges. Every other edge keeps its normal styling.
- Group nodes into `subgraph cluster_N { ... }` blocks by **current top-level `src/`
  directory** — re-derive this list from the actual tree each run (see step 1's warning
  about stale layout) rather than trusting a remembered list; as of this writing that's
  `Application`, `Board`, `Component`, `Domain`, `Platform`, `Support`, `Target`, `Type`,
  `Utility` (`Component` and `Domain` are themselves renames — formerly `Device` and
  `Dictionary` — so don't be surprised if they've moved again by the next run). Each
  cluster `style=dashed, color="#c4cdc3"`. Color at the **node** level (not the cluster
  polygon, which stays unfilled in this style) by sub-area, reusing the exact palette from
  `doc/module_dependencies.svg`'s current source if you still have it: blue `fill="#e4ecf7"
  stroke="#31537a"` for `Application/*`; copper `fill="#f7ecdf" stroke="#8a5a30"` for
  `Board/*`; purple `fill="#f1e4f7" stroke="#6a3a7a"` for `Component/*`; green
  `fill="#e4f7ec" stroke="#1b6f60"` for `Domain/*`; a slightly different green
  `fill="#dff3e6" stroke="#1b6f60"` for `Platform/*`; darker copper `fill="#f7f0e4"
  stroke="#8a5a18"` for the `Support/Plumbing*` nodes specifically; neutral grey
  `fill="#eef1ee" stroke="#455148"` for everything else under `Support` (`Console`,
  `Repository`), plus `Type`, `Type/Abstract`, and `Utility`; red `fill="#f7e4e4"
  stroke="#8a3030"` for `Target/*`. If a future rename shuffles directories again, keep
  each sub-area's color attached to *what it represents* (e.g. Plumbing stays copper)
  rather than to whatever top-level cluster it happens to sit in that day.
- No legend node/cluster — tried once, rejected by the user as visual clutter. The
  solid/dashed distinction is explained only in the graph `label` text (below); don't
  re-add a legend unless asked.
- Graph `label` at the bottom (`labelloc=b, fontsize=13`), naming the actual preset used
  and explaining all three edge treatments (kind plus the two-way override):
  `"Module dependency graph — one node per source directory actually built by CMake\n(preset V71_hello_world: APP=Hello_world, TARGET=V71_EK_hello_world, BOARD=V71_EK, PLATFORM=SamV71)   (arrow: module → module it depends on)\nsolid = #include dependency     dashed = C++20 module import     bold red = two-way (circular) dependency between the pair"`

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
  with a line of context above it that mentions both `#include` and `import` are covered,
  briefly says what the current `import` edges are (as of this writing: consumers of
  `Support/Console`'s `Support.Console_service` module) since that list is short enough to
  be worth naming rather than only gesturing at, and notes that a module pair with edges in
  both directions is flagged bold red.

Leave the rest of that section (and the `## Wiring` section above it) untouched.
