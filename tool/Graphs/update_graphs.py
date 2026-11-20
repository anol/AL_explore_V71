"""Regenerate every generated graph in doc/ from the sources.

    python tool/Graphs/update_graphs.py [--preset Hello_AstraLEO_EM] [--dot PATH] [--detailed]

Writes doc/module_dependencies.svg, doc/top_level_dependencies.svg and the class graphs listed in
class_graph.GRAPHS, for the files that CMake compiles with the given configure preset. With --detailed the
class graphs are written as doc/<name>_detailed.svg instead, with inheritance edges and dashed pointer
aggregation. Needs Graphviz (`dot` on PATH, or under Program Files).
"""
import argparse
import glob
import os
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from cmake_sources import active_files, preset_variables  # noqa: E402
from class_graph import class_graphs  # noqa: E402
from module_graph import analyze, module_dot, top_level_dot  # noqa: E402

REPO = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))


def find_dot(explicit):
    if explicit:
        return explicit
    found = shutil.which('dot')
    if found:
        return found
    for pattern in (r'C:\Program Files\Graphviz*\bin\dot.exe', r'C:\Program Files (x86)\Graphviz*\bin\dot.exe'):
        hits = sorted(glob.glob(pattern))
        if hits:
            return hits[-1]
    raise SystemExit('Graphviz dot not found: put it on PATH or pass --dot')


def render(dot, text, svg_path):
    result = subprocess.run([dot, '-Kdot', '-Tsvg', '-o', svg_path], input=text.encode('utf-8'),
                            capture_output=True)
    if result.returncode != 0:
        raise SystemExit(f'dot failed for {svg_path}:\n{result.stderr.decode(errors="replace")}')
    with open(svg_path, encoding='utf-8') as f:
        if 'foreignObject' in f.read():  # would not render in some markdown previewers
            raise SystemExit(f'{svg_path} contains foreignObject')


def main():
    sys.stdout.reconfigure(encoding='utf-8')
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument('--preset', default='Hello_AstraLEO_EM', help='CMake configure preset')
    parser.add_argument('--dot', help='path to Graphviz dot')
    parser.add_argument('--detailed', action='store_true',
                        help='class graphs with inheritance edges and dashed pointers, as doc/<name>_detailed.svg')
    args = parser.parse_args()

    dot = find_dot(args.dot)
    variables = preset_variables(REPO, args.preset)
    active = active_files(REPO, variables)
    doc = os.path.join(REPO, 'doc')

    result = analyze(os.path.join(REPO, 'src'), active)
    render(dot, module_dot(result, args.preset, variables), os.path.join(doc, 'module_dependencies.svg'))
    top_text, top_two_way = top_level_dot(result, args.preset, variables)
    render(dot, top_text, os.path.join(doc, 'top_level_dependencies.svg'))
    print(f'module_dependencies: {len(result["modules"])} modules, {len(result["edges"])} edges '
          f'({len(result["include"])} #include)')
    print('  two-way pairs:', ', '.join(f'{a} <-> {b}' for a, b in sorted(result['two_way']) if a < b) or 'none')
    print('  loops:', ' | '.join(' -> '.join(c + [c[0]]) for c in result['loops']) or 'none')
    print('top_level_dependencies: two-way pairs:',
          ', '.join(f'{a} <-> {b}' for a, b in top_two_way if a < b) or 'none')
    for w in result['warnings']:
        print('  warning:', w)

    for name, text, n_nodes, n_edges in class_graphs([f for fs in active.values() for f in fs], args.detailed):
        render(dot, text, os.path.join(doc, name + '.svg'))
        print(f'{name}: {n_nodes} classes, {n_edges} edges')


if __name__ == '__main__':
    main()
