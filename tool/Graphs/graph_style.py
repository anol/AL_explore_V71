"""Graphviz style shared by the class graphs (composition / aggregation)."""

COMPOSITION = 'arrowtail=diamond, dir=both, arrowhead=none, color="#1b6f60", fontcolor="#155a4e"'
AGGREGATION = 'arrowtail=odiamond, dir=both, arrowhead=none, color="#b8631f", fontcolor="#8a4c18"'
POINTER = AGGREGATION + ', style=dashed'  # --detailed: aggregation through a pointer (may be null / reassigned)
INHERITANCE = ('arrowtail=none, dir=forward, arrowhead=empty, color="#3a3a3a", fontcolor="#3a3a3a", '
               'penwidth=1.1, style=solid')  # --detailed: derived -> base, hollow triangle at the base
EDGE_STYLES = {'comp': COMPOSITION, 'aggr': AGGREGATION, 'ptr': POINTER, 'inherit': INHERITANCE}


def _escape(s):
    return s.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;')


def _label(lines):
    """[stereotype / base class lines in small italics..., class name]"""
    *small, name = lines
    parts = [f'<I><FONT POINT-SIZE="11">{_escape(x)}</FONT></I><BR/>' for x in small]
    return '<' + ''.join(parts) + _escape(name) + '>'


def class_graph_dot(graph_name, nodes, edges, title):
    """nodes: [[id, label lines]],
    edges: [[from, to, member name, 'comp' | 'aggr' | 'ptr' | 'inherit']] -> .dot text"""
    o = [f'digraph {graph_name} {{', '  rankdir=TB;',
         '  node [shape=box, fontname="Consolas", fontsize=15];',
         '  edge [fontname="Consolas", fontsize=11];']
    for nid, lines in nodes:
        o.append(f'  "{nid}" [label={_label(lines)}];')
    for a, b, member, kind in edges:
        label = '' if kind == 'inherit' else f', label=" {member}"'
        o.append(f'  "{a}" -> "{b}" [{EDGE_STYLES[kind]}{label}];')
    o.append(f'  labelloc=b; fontsize=13; fontname="Consolas"; label="{title}";')
    o.append('}')
    return '\n'.join(o) + '\n'
