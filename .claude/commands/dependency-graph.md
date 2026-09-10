---
description: Regenerate the module (directory-level) dependency graph and link it from README.md
---

Regenerate `doc/module_dependencies.svg` — the directory-level `#include` dependency
graph — matching the approach already used for it in this repo (see recent conversation
history / git log for the original derivation).

Do **not** also generate a header-file-level (per-.h-file) graph — that was tried and
rejected as too unreadable (88 nodes, extremely wide, unreadable at a glance). Only the
module-level rollup is wanted.

## 1. Extract `#include` edges

- Enumerate every `.h`, `.hpp`, `.c`, `.cpp` under `src/`.
- Under `src/Target/`, include **only** `src/Target/V71_EK_hello_world/**` — the other
  target directories (`Win_hello_world`, `Win_ethernet_test`, `V71_ethernet_test`,
  `Unit_test`) are alternate/mock build variants the user doesn't want in this graph;
  exclude them entirely (files, and any modules that would otherwise only appear because
  of them).
- Grep every quoted `#include "..."` (ignore commented-out ones, e.g. `// #include "x.h"`).
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

## 2. Roll up to module level

A module = one source directory under `src/` (e.g. `Application/Hello_world`,
`Platform/SamV71_platform` — leaf directories, not collapsed further up). For every
resolved file-level edge, map both endpoints to their containing directory, dedupe, and
drop same-module edges (a directory including its own sibling file isn't a *module*
dependency).

## 3. Render with Graphviz — match the existing style

- `rankdir=LR`, natural size (no forced `size`/`ratio=fill` — this is a browse/zoom
  reference, not a print deliverable)
- `node [shape=box, style="rounded,filled", fontname="Consolas", fontsize=13]`
- `edge [color="#7a8a80", arrowsize=0.8]`
- Group nodes into `subgraph cluster_N { ... }` blocks by top-level directory
  (`Abstract`, `Application`, `Board`, `Device`, `Dictionary`, `Platform`, `Plumbing`,
  `Repository`, `Target`, `Utility`), each cluster `style=dashed, color="#c4cdc3"`, with
  a light `fillcolor` per top-level area (blue-ish for Application, green-ish for
  Platform, copper for Board/Plumbing, red-ish for Target, purple for Device, neutral
  grey for Abstract/Repository/Utility/Dictionary) — reuse the exact palette from
  `doc/module_dependencies.svg`'s current source if you still have it, otherwise pick
  something in that spirit.
- Graph `label` at the bottom (`labelloc=b, fontsize=13`):
  `"Module dependency graph — one node per source directory under src/   (arrow: module → module it includes from)"`

Render with `dot -Kdot -Tsvg` (`dot.exe` at `C:\Program Files\Graphviz 2.44.1\bin\`).
Verify zero `foreignObject` elements in the output (`grep -c foreignObject`) — should be
automatic with Graphviz's own text rendering, but it's the check that would have caught
the Mermaid-based diagram that failed to render in this user's markdown previewer earlier
in this project's history.

## 4. Save and embed it

- Overwrite `doc/module_dependencies.svg`.
- Confirm (or add, if missing) an **embedded image** of it — not a plain link; a
  plain-text-only Mermaid SVG once failed to render inline in the user's previewer, but
  that's not a concern with Graphviz's plain-`<text>` output, and the user explicitly
  wants it visible without a click — under the `## Dependencies` section of the
  top-level `README.md`: `![Module dependency graph](doc/module_dependencies.svg)`,
  with a line of context above it.

Leave the rest of that section (and the `## Wiring` section above it) untouched.
