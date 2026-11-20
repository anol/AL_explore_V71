# Graphs

Regenerates the generated graphs in `doc/` from the sources:

```sh
python tool/Graphs/update_graphs.py            # or double-click update_graphs.bat
python tool/Graphs/update_graphs.py --preset Hello_AstraLEO_EM --dot "C:\Program Files\Graphviz 2.44.1\bin\dot.exe"
python tool/Graphs/update_graphs.py --detailed # class graphs as doc/<name>_detailed.svg, see below
```

Requires Python 3 and [Graphviz](https://graphviz.org/) (`dot` on `PATH`, or under `Program Files`).
Only the files that CMake compiles with the chosen configure preset (default `Hello_AstraLEO_EM`) are
read: the preset's cache variables come from `CMakePresets.json`, and the `add_subdirectory()` /
`target_sources()` calls are followed from the root `CMakeLists.txt`. Files commented out in
`target_sources()` are left out; headers in an active directory count even when not listed.

| Output | Built by | Shows |
|---|---|---|
| `module_dependencies.svg` | `module_graph.py` | One node per source directory; `#include` (solid) and C++20 `import` (dashed) dependencies; two-way pairs bold red, loops through 3+ modules bold purple |
| `top_level_dependencies.svg` | `module_graph.py` | The same, rolled up to the top-level `src/` directories |
| `*_composition.svg` | `class_graph.py` | What a class owns, recursively (members held by value) |
| `*_aggregation.svg` | `class_graph.py` | Owned members plus references and raw pointers to project classes |
| `*_detailed.svg` (`--detailed`) | `class_graph.py` | The same class graphs, but every base class is a box of its own with a derived → base edge (hollow triangle, UML inheritance), and aggregation through a pointer is dashed (may be null or reassigned) while a reference stays solid. Written next to the plain graphs, not instead of them; the README embeds only the plain ones |

To graph another class, add it to `class_graph.GRAPHS` (output name, class name, aggregation or
composition only), run the tool, and embed `doc/<name>.svg` in the README.

The class graphs and their root classes are listed in `class_graph.GRAPHS`. They are derived from the
class declarations; see the docstring of `class_graph.py` for the rules. Data records and messages of
the instruction plumbing (`class_graph.EXCLUDED`) are left out on purpose. Every graph is checked for
`foreignObject` elements, which some markdown previewers cannot render.

`doc/Bias Voltage Control.svg` is drawn by hand (draw.io) and is not touched.

Graphviz 2.44 writes the cluster boxes of `module_dependencies.svg` in a different order on every run,
so that file can show up as changed in git even when the graph itself is the same.
