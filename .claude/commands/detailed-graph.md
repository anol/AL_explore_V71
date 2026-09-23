---
description: Generate a detailed Graphviz aggregation-graph SVG for one C++ class (composition, aggregation by reference/pointer, and inheritance) and link it from README.md
argument-hint: <ClassName>
---

Generate a **detailed** aggregation graph for the class `$ARGUMENTS`, matching the style
already established in `doc/hello_world_aggregation.svg` and
`doc/v71_ek_board_aggregation.svg`. This is the "detailed" variant of `/aggregation-graph`:
it additionally draws real inheritance edges (instead of a text-only base-class line) and
separates pointer-based aggregation from reference-based aggregation.

## 1. Trace the relationships

Find the class `$ARGUMENTS` in the source. Starting from it, recursively follow:

- **composition** — its own data members owned by value (e.g. `Foo the_foo{};`)
- **aggregation by reference** — references it receives via constructor injection and
  holds onto (e.g. `Foo& use_foo`)
- **aggregation by pointer** — raw pointer data members that point at another *project*
  class (e.g. `Foo *optional_foo;`), however they got set (constructor injection or
  assigned later via a setter) — see the exclusions below for which pointers don't count
- **inheritance** — its base class(es), read from the class declaration itself
  (`class X : public Base`), not guessed from what it implements

Keep following composition, aggregation and inheritance edges outward from each
newly-found class, the same way, until you hit a leaf: an interface/abstract class (its
own base, if it has one, still gets an edge — interfaces can inherit from other
interfaces — but don't expand its own members: they're empty by convention anyway) or a
concrete class with no further members worth tracing (leaf-level utility state).

Do **not** expand an interface into whichever concrete class implements it elsewhere —
that implementation belongs to a *different* class's own aggregation graph, not this one.

**Pointer exclusions** — do not draw an edge for a pointer member when:
- it's untyped or points at a non-project type (`void *`, SDK/RTOS opaque handles like
  `QueueHandle_t`, hardware register structs like `pio_registers_t *`, raw buffers like
  `uint8_t *`) — there's no project class to point the edge at
- it's a `static` member used for a singleton/ISR-lookup pattern (e.g.
  `static SamV71_SPI *optional_SPI0_driver;`) — that's global bookkeeping, not part of
  this instance's own aggregation structure, and typically self-referential anyway

## 2. Render it with Graphviz — match the existing style exactly

Reference `doc/v71_ek_board_aggregation.svg`'s source (reconstruct the `.dot` from it, or
check recent conversation history) for the precise conventions:

- `rankdir=TB`, natural size (no forced `size`/`ratio=fill` — this is for a README, not print)
- node style: `shape=box, fontname="Consolas", fontsize=15`; interfaces/abstract classes
  get a `«interface»`/`«abstract»` stereotype as the first line of the label, in small
  italic font (`<I><FONT POINT-SIZE="11">…</FONT></I>`)
- No text-only super-class line above the class name — a base class always gets its own
  node plus a real inheritance edge instead (see below), even when nothing else in the
  graph would otherwise reference it
- Template arguments are kept as written in the declaration (e.g.
  `Abstract_queue<T>` for the generic interface, `FreeRTOS_queue<void *, Queue_size>` for
  the concrete instantiation), which needs an HTML-like label
  (`label=<...>`, with `<`/`>` in names escaped as `&lt;`/`&gt;`); it still renders as
  plain `<text>`, so the `foreignObject` check keeps passing
- edge style: `fontname="Consolas", fontsize=11`
- **composition**: `arrowtail=diamond, dir=both, arrowhead=none, color="#1b6f60", fontcolor="#155a4e"`,
  labeled with the member name (e.g. `the_histogram`)
- **aggregation by reference**: `arrowtail=odiamond, dir=both, arrowhead=none, color="#b8631f", fontcolor="#8a4c18"`
  (solid), labeled with the member/parameter name (e.g. `use_board`)
- **aggregation by pointer**: same as aggregation by reference but **dashed**
  (`style=dashed` added), labeled with the member name (e.g. `optional_request`) — the
  dashing is what visually distinguishes "points at, may be null, may be reassigned" from
  a reference's "bound once, never null"
- **inheritance**: `arrowtail=none, dir=forward, arrowhead=empty, color="#3a3a3a",
  fontcolor="#3a3a3a", penwidth=1.1, style=solid`, no label, drawn from the derived class
  to its base (hollow UML triangle at the base end)
- graph `label` at the bottom (`labelloc=b, fontsize=13`):
  `"<ClassName> — aggregation graph   [ ◇ aggregation (non-owning ref/pointer, dashed)    ◆ composition (owned member)    △ inheritance (is-a) ]"`

Render with `dot -Kdot -Tsvg` (the `dot.exe` at `C:\Program Files\Graphviz 2.44.1\bin\`).
**Verify the output has zero `foreignObject` elements** (`grep -c foreignObject` on the
result) — Graphviz's own text rendering is plain `<text>` by default and should pass this
automatically; that check exists because a Mermaid-based diagram would fail it, and one
already did in this repo's history (it didn't render in the user's markdown previewer).

## 3. Save and embed it

- Save as `doc/<snake_case_class_name>_aggregation.svg` (e.g. `Request_router` →
  `doc/request_router_aggregation.svg`) — same filename convention as `/aggregation-graph`,
  since this is a richer rendering of the same diagram, not a separate artifact
- Add it under the `## Wiring` section of the top-level `README.md`, alongside the
  existing ones, as an **embedded image** (not a plain link — a plain-text-only Mermaid
  SVG once failed to render inline in the user's previewer, but that's not a concern
  with Graphviz's plain-`<text>` output, and the user explicitly wants these visible
  without a click):
  a short line of context, then `![<ClassName> aggregation graph](doc/<file>.svg)` on
  its own line.

Keep the existing entries in that section as they are; only add to them.
