"""Module (source directory) dependency graphs: #include edges and C++20 module import edges.

- An #include resolves to a project file by basename; names that match nothing in the project are
  external (FreeRTOS, SAM SDK, ...). An ambiguous basename prefers the includer's own directory.
- `import X;`, `export import X;` and module implementation units (`module X;`) depend on the
  directory of the file that has `export module X;`. Partitions stay inside their module.
- Same-directory edges are dropped. Two-way pairs and loops through 3+ modules are flagged.
"""
import os
import re
from collections import defaultdict

DEPENDENCY_LEGEND = ('solid = #include dependency     dashed = C++20 module import     '
                     'bold red = two-way (circular) dependency between the pair')


def _strip_comments(t):
    t = re.sub(r'/\*.*?\*/', lambda m: '\n' * m.group(0).count('\n'), t, flags=re.S)
    return re.sub(r'//[^\n]*', '', t)


def analyze(src, active):
    """src: the src/ directory, active: {dir: files} -> dict with modules, edges, loops, ..."""
    rel = lambda p: os.path.relpath(p, src).replace('\\', '/')
    modules = sorted(rel(d) for d in active)
    files = [f for fs in active.values() for f in fs]
    by_base = defaultdict(list)
    for f in files:
        by_base[os.path.basename(f)].append(f)

    inc, imp, warnings = set(), set(), []
    exports, imports = {}, []
    for f in files:
        with open(f, encoding='utf-8', errors='replace') as fh:
            t = _strip_comments(fh.read())
        here = rel(os.path.dirname(f))
        for m in re.finditer(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', t, re.M):
            base = os.path.basename(m.group(1))
            cands = by_base.get(base, [])
            if not cands:
                continue  # external header
            if len(cands) > 1:
                same = [x for x in cands if os.path.dirname(x) == os.path.dirname(f)]
                if len(same) != 1:
                    warnings.append(f'ambiguous {base} from {rel(f)}')
                    continue
                cands = same
            if os.path.normpath(cands[0]) == os.path.normpath(f):
                continue
            there = rel(os.path.dirname(cands[0]))
            if there != here:
                inc.add((here, there))
        for m in re.finditer(r'^\s*export\s+module\s+([\w.]+)\s*;', t, re.M):
            exports[m.group(1)] = here
        for m in re.finditer(r'^\s*(?:export\s+)?import\s+([\w.]+)\s*;', t, re.M):
            imports.append((here, m.group(1), rel(f)))
        for m in re.finditer(r'^\s*module\s+([\w.]+)\s*;', t, re.M):
            imports.append((here, m.group(1), rel(f)))
    for here, name, f in imports:
        there = exports.get(name)
        if there is None:
            if not name.startswith('std'):
                warnings.append(f'unresolved import {name} in {f}')
            continue
        if there != here:
            imp.add((here, there))

    edges = inc | imp
    two_way = {(a, b) for a, b in edges if (b, a) in edges}

    # every simple cycle of 3+ modules, each found once (from its smallest node)
    adj = defaultdict(set)
    for a, b in edges:
        adj[a].add(b)
    index = {n: i for i, n in enumerate(modules)}
    loops = []

    def dfs(start, cur, path, on_path):
        for n in adj[cur]:
            if index.get(n, -1) < index[start]:
                continue
            if n == start:
                if len(path) >= 3:
                    loops.append(list(path))
            elif n not in on_path:
                on_path.add(n)
                path.append(n)
                dfs(start, n, path, on_path)
                path.pop()
                on_path.discard(n)

    for s in modules:
        dfs(s, s, [s], {s})
    return {'modules': modules, 'include': inc, 'edges': edges, 'two_way': two_way,
            'loops': loops, 'warnings': warnings}


def _module_colors(m):
    if m.startswith('Application'): return '#e4ecf7', '#31537a'
    if m.startswith('Board'): return '#f7ecdf', '#8a5a30'
    if m.startswith('Component'): return '#f1e4f7', '#6a3a7a'
    if m.startswith('Domain'): return '#e4f7ec', '#1b6f60'
    if m.startswith('Platform'): return '#dff3e6', '#1b6f60'
    if m.startswith('Support/Plumbing'): return '#f7f0e4', '#8a5a18'
    if m.startswith('Target'): return '#f7e4e4', '#8a3030'
    return '#eef1ee', '#455148'


def _preset_line(preset, variables):
    keys = [k for k in ('APP', 'TARGET', 'BOARD', 'PLATFORM') if k in variables]
    return f'(preset {preset}: ' + ', '.join(f'{k}={variables[k]}' for k in keys) + ')'


def module_dot(result, preset, variables):
    loop_edges = {(c[i], c[(i + 1) % len(c)]) for c in result['loops'] for i in range(len(c))}
    out = ['digraph ModuleDependencies {', '  rankdir=LR;',
           '  node [shape=box, style="rounded,filled", fontname="Consolas", fontsize=13];',
           '  edge [color="#7a8a80", arrowsize=0.8];']
    groups = defaultdict(list)
    for m in result['modules']:
        groups[m.split('/')[0]].append(m)
    for i, (g, ms) in enumerate(sorted(groups.items())):
        out.append(f'  subgraph cluster_{i} {{ label="{g}"; style=dashed; color="#c4cdc3"; fontname="Consolas"; fontsize=14;')
        for m in ms:
            fill, stroke = _module_colors(m)
            out.append(f'    "{m}" [fillcolor="{fill}", color="{stroke}"];')
        out.append('  }')
    for a, b in sorted(result['edges']):
        dashed = (a, b) not in result['include']
        if (a, b) in result['two_way']:
            attr = 'color="#c0392b", penwidth=2.2'
        elif (a, b) in loop_edges:
            attr = ('style=dashed, ' if dashed else '') + 'color="#8e24aa", penwidth=2.2'
        else:
            attr = 'style=dashed, color="#3a6ea5", penwidth=1.3' if dashed else ''
        out.append(f'  "{a}" -> "{b}"' + (f' [{attr}]' if attr else '') + ';')
    loops = ' | '.join(' → '.join(c + [c[0]]) for c in result['loops']) or 'none'
    label = ('Module dependency graph — one node per source directory actually built by CMake\\n'
             f'{_preset_line(preset, variables)}   (arrow: module → module it depends on)\\n'
             f'{DEPENDENCY_LEGEND}\\n'
             'bold purple = part of a dependency loop through 3 or more modules\\n'
             f'Loops found: {loops}')
    out.append(f'  labelloc=b; fontsize=13; fontname="Consolas"; label="{label}";')
    out.append('}')
    return '\n'.join(out) + '\n'


def top_level_dot(result, preset, variables):
    top = lambda m: m.split('/')[0]
    inc = {(top(a), top(b)) for a, b in result['include'] if top(a) != top(b)}
    edges = {(top(a), top(b)) for a, b in result['edges'] if top(a) != top(b)}
    colors = {'Application': ('#e4ecf7', '#31537a'), 'Board': ('#f7ecdf', '#8a5a30'),
              'Component': ('#f1e4f7', '#6a3a7a'), 'Domain': ('#e4f7ec', '#1b6f60'),
              'Platform': ('#dff3e6', '#1b6f60'), 'Target': ('#f7e4e4', '#8a3030')}
    o = ['digraph TopLevelDependencies {', '  rankdir=LR;',
         '  node [shape=box, style="rounded,filled", fontname="Consolas", fontsize=13];',
         '  edge [color="#7a8a80", arrowsize=0.8];']
    for n in sorted({top(m) for m in result['modules']}):
        fill, stroke = colors.get(n, ('#eef1ee', '#455148'))
        o.append(f'  "{n}" [fillcolor="{fill}", color="{stroke}"];')
    two_way = {e for e in edges if (e[1], e[0]) in edges}
    for a, b in sorted(edges):
        if (a, b) in two_way:
            attr = ' [color="#c0392b", penwidth=2.2]'
        elif (a, b) in inc:
            attr = ''
        else:
            attr = ' [style=dashed, color="#3a6ea5", penwidth=1.3]'
        o.append(f'  "{a}" -> "{b}"{attr};')
    label = ('Top-level module dependency graph -- one node per top-level src/ directory\\n'
             f'{_preset_line(preset, variables)}\\n'
             '(arrow: directory -> directory it depends on)\\n'
             'solid = #include dependency     dashed = C++20 module import\\n'
             'bold red = two-way (circular) dependency between the pair\\n'
             'Rolled up from doc/module_dependencies.svg (per-source-directory graph).')
    o.append(f'  labelloc=b; fontsize=13; fontname="Consolas"; label="{label}";')
    o.append('}')
    return '\n'.join(o) + '\n', sorted(two_way)
