---
description: Generate a Graphviz aggregation-graph SVG for one C++ class and link it from README.md
argument-hint: <ClassName>
---

Generate an aggregation graph for the class `$ARGUMENTS`, matching the style already
established in `doc/hello_world_aggregation.svg` and `doc/v71_ek_board_aggregation.svg`.

## 1. Trace the relationships

Find the class `$ARGUMENTS` in the source. Starting from it, recursively follow:

- **composition** — its own data members (owned by value, e.g. `Foo the_foo{};`)
- **aggregation** — references/pointers it receives via constructor injection
  (e.g. `Foo& use_foo`) and holds onto
- **inherits / realizes** — its base class, and any `Abstract_*`/interface base
  classes it implements

Keep following composition and aggregation edges outward from each newly-found class,
the same way, until you hit a leaf: an interface/abstract class (don't expand further —
stop there, shown just as a stereotype box) or a class with no further members worth
tracing (leaf-level utility state).

Do **not** expand an interface into whichever concrete class implements it elsewhere —
that implementation belongs to a *different* class's own aggregation graph, not this one.

## 2. Render it with Graphviz — match the existing style exactly

Reference `doc/hello_world_aggregation.svg`'s source (reconstruct the `.dot` from it, or
check recent conversation history) for the precise conventions:

- `rankdir=TB`, natural size (no forced `size`/`ratio=fill` — this is for a README, not print)
- node style: `shape=box, fontname="Consolas", fontsize=15`; interfaces/abstract classes
  get a `«interface»`/`«abstract»` stereotype as the first line of the label
- edge style: `fontname="Consolas", fontsize=11`
- **aggregation**: `arrowtail=odiamond, dir=both, arrowhead=none, color="#b8631f", fontcolor="#8a4c18"`,
  labeled with the member/parameter name (e.g. `use_board`)
- **composition**: `arrowtail=diamond, dir=both, arrowhead=none, color="#1b6f60", fontcolor="#155a4e"`,
  labeled with the member name (e.g. `the_histogram`)
- **inherits/realizes**: `arrowhead=empty`; solid line for inheritance, `style=dashed` for
  interface realization
- graph `label` at the bottom (`labelloc=b, fontsize=13`):
  `"<ClassName> — aggregation graph   [ → inherits/realizes    ◇ aggregation (non-owning ref)    ◆ composition (owned member) ]"`

Render with `dot -Kdot -Tsvg` (the `dot.exe` at `C:\Program Files\Graphviz 2.44.1\bin\`).
**Verify the output has zero `foreignObject` elements** (`grep -c foreignObject` on the
result) — Graphviz's own text rendering is plain `<text>` by default and should pass this
automatically; that check exists because a Mermaid-based diagram would fail it, and one
already did in this repo's history (it didn't render in the user's markdown previewer).

## 3. Save and embed it

- Save as `doc/<snake_case_class_name>_aggregation.svg` (e.g. `Request_router` →
  `doc/request_router_aggregation.svg`)
- Add it under the `## Wiring` section of the top-level `README.md`, alongside the
  existing ones, as an **embedded image** (not a plain link — a plain-text-only Mermaid
  SVG once failed to render inline in the user's previewer, but that's not a concern
  with Graphviz's plain-`<text>` output, and the user explicitly wants these visible
  without a click):
  a short line of context, then `![<ClassName> aggregation graph](doc/<file>.svg)` on
  its own line.

Keep the existing entries in that section as they are; only add to them.
