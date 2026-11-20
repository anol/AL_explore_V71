"""Class wiring graphs, derived from the class declarations of the compiled sources.

From each root class, members are followed outward:
- composition: a data member held by value (also arrays), drawn  ◆  and labelled with the member name;
  every owned instance gets its own box, so two owners of the same type are not shown sharing one object
- aggregation (aggregation graphs only): a reference or raw pointer to a project class, drawn  ◇ ;
  it points at the one instance of that type already in the graph, or at a box of its own
- interfaces / abstract classes (classes with pure virtual functions) are leaves; their members are not
  followed, and a concrete class is never replaced by the class implementing an interface elsewhere
- base classes are shown as the small italic line above the class name, «interface» / «abstract» above that
- static members, non-project types (SDK, RTOS handles, builtins) and EXCLUDED are left out;
  class-scope `using` aliases are resolved
"""
import os
import re
from collections import defaultdict

from graph_style import class_graph_dot

# (output name, root class, aggregation?)
GRAPHS = [
    ('hello_world_composition', 'Hello_world', False),
    ('v71_ek_board_composition', 'V71_EK_board', False),
    ('service_provider_aggregation', 'Service_provider', True),
    ('spectranode_console_aggregation', 'SpectraNode_console', True),
    ('samv71_spi_aggregation', 'SamV71_SPI', True),
    ('samv71_i2c_aggregation', 'SamV71_I2C', True),
    ('samv71_usart_aggregation', 'SamV71_USART', True),
    ('samv71_adc_aggregation', 'SamV71_ADC', True),
]

# Data records and messages passed through the instruction plumbing: real members, but not wiring.
EXCLUDED = {'Argument', 'Attribute_type', 'Instruction_token', 'Instruction_major'}

COMPOSITION_TITLE = '{} — composition graph   [ ◆ composition (owned member) ]'
AGGREGATION_TITLE = '{} — aggregation graph   [ ◇ aggregation (non-owning ref)    ◆ composition (owned member) ]'

_HEADER = re.compile(r'(?<![\w:])(class|struct)\s+(?:\[\[[^\]]*\]\]\s*)?(\w+)\s*(?:final\s*)?(:\s*[^{;()]*?)?\s*\{')
_ACCESS = re.compile(r'^\s*(public|private|protected)\s*:\s*')
_QUALIFIERS = re.compile(r'\b(const|volatile|mutable|inline|constexpr|typename)\b')


# ---------------------------------------------------------------- parsing
def _clean(text):
    text = re.sub(r'/\*.*?\*/', ' ', text, flags=re.S)
    text = re.sub(r'//[^\n]*', '', text)
    text = re.sub(r'"(?:\\.|[^"\\])*"', '""', text)
    text = re.sub(r"'(?:\\.|[^'\\])'", "' '", text)
    return re.sub(r'^\s*#[^\n]*', '', text, flags=re.M)


def _match_brace(t, i):
    """t[i] == '{' -> index just past the matching '}'"""
    depth = 0
    for j in range(i, len(t)):
        if t[j] == '{':
            depth += 1
        elif t[j] == '}':
            depth -= 1
            if depth == 0:
                return j + 1
    return len(t)


def _namespace_marks(t):
    """[(position, enclosing namespace path)] from a brace scan"""
    marks, stack, depth = [], [], 0
    for m in re.finditer(r'\bnamespace\s+([\w:]+)\s*\{|[{}]', t):
        if m.group(1):
            depth += 1
            stack.append((depth, m.group(1)))
        elif m.group(0) == '{':
            depth += 1
        else:
            if stack and stack[-1][0] == depth:
                stack.pop()
            depth -= 1
        marks.append((m.end(), '::'.join(n for _, n in stack)))
    return marks


def _members(body):
    """depth-0 data members [(type, '' | '*' | '&', name)] and `using` aliases of a class body"""
    statements, cur, i = [], '', 0
    while i < len(body):
        c = body[i]
        if c == '{':
            end = _match_brace(body, i)
            stmt = _ACCESS.sub('', cur).strip()
            if stmt and '(' not in stmt and not re.match(r'(enum|class|struct|union|namespace)\b', stmt) \
                    and re.search(r'\w\s*(\[[^\]]*\])?\s*$', stmt):
                cur += body[i:end]  # brace initializer of a data member
                i = end
                continue
            cur = ''  # function body, nested type, ...
            i = end
            if i < len(body) and body[i] == ';':
                i += 1
            continue
        if c == ';':
            statements.append(cur)
            cur = ''
        elif c == ':' and cur.strip() in ('public', 'private', 'protected'):
            cur = ''
        else:
            cur += c
        i += 1
    members, aliases = [], {}
    for stmt in statements:
        s = _ACCESS.sub('', stmt).strip()
        a = re.match(r'using\s+(\w+)\s*=\s*(.+)$', s, re.S)
        if a:
            aliases[a.group(1)] = re.sub(r'\s+', ' ', _QUALIFIERS.sub('', a.group(2))).strip()
            continue
        if not s or '(' in s.split('{')[0].split('=')[0]:
            continue  # functions, function pointers
        if re.match(r'(using|typedef|friend|static|enum|template|class|struct|union|namespace|return|operator)\b', s):
            continue
        s = re.sub(r'\{.*$', '', s, flags=re.S)
        s = re.sub(r'=.*$', '', s, flags=re.S)
        s = _QUALIFIERS.sub('', s).strip()
        m = re.match(r'^(?P<type>.+?)\s*(?P<ptr>[*&]?)\s*(?P<name>\w+)\s*(?P<arr>\[[^\]]*\])?$', s, re.S)
        if not m:
            continue
        ty, ptr = re.sub(r'\s+', ' ', m.group('type')).strip(), m.group('ptr')
        if ty.endswith(('*', '&')):
            ptr, ty = ty[-1], ty[:-1].strip()
        members.append((ty, ptr, m.group('name')))
    return members, aliases


def parse_classes(files):
    """{simple class name: [class info]} for every class / struct defined in files"""
    classes = defaultdict(list)
    for path in sorted(files):
        if not path.endswith(('.h', '.hpp', '.cppm', '.cpp')):
            continue
        with open(path, encoding='utf-8', errors='replace') as f:
            t = _clean(f.read())
        marks = _namespace_marks(t)
        for m in _HEADER.finditer(t):
            if re.search(r'\benum\s*$', t[max(0, m.start() - 12):m.start()]):
                continue  # enum class
            start = m.end() - 1
            body = t[start + 1:_match_brace(t, start) - 1]
            ns = ''
            for pos, n in marks:
                if pos <= m.start():
                    ns = n
            bases = []
            if m.group(3):
                for b in re.split(r',(?![^<]*>)', m.group(3)[1:]):
                    b = re.sub(r'\b(public|private|protected|virtual)\b', '', b).strip()
                    if b:
                        bases.append(b)
            members, aliases = _members(body)
            classes[m.group(2)].append({
                'name': m.group(2), 'ns': ns, 'path': path, 'bases': bases, 'members': members,
                'aliases': aliases,
                'pure': bool(re.search(r'\)\s*(const\s*)?(noexcept\s*)?(override\s*)?=\s*0\s*;', body)),
            })
    return classes


# ---------------------------------------------------------------- graphs
def _unqualify(type_text):
    """Abstract::Abstract_provider<X> -> Abstract_provider<X> (qualifiers inside <> are kept)"""
    head, sep, tail = type_text.partition('<')
    return head.split('::')[-1].strip() + sep + tail


def _resolve(classes, type_text, owner):
    """type text as written in owner -> class info of a project class, or None"""
    parts = type_text.split('<')[0].strip().split('::')
    cands = classes.get(parts[-1], [])
    if len(cands) > 1 and len(parts) > 1:  # a qualifier picks the namespace
        cands = [c for c in cands if c['ns'].split('::')[-1] == parts[-2]] or cands
    if len(cands) > 1 and owner:  # else prefer the owner's directory
        cands = [c for c in cands if os.path.dirname(c['path']) == os.path.dirname(owner['path'])] or cands
    if len(cands) > 1:  # several definitions of one name: the richest one
        cands = sorted(cands, key=lambda c: -len(c['members']))
    return cands[0] if cands else None


def _label(c, display):
    lines = []
    if c['pure']:
        lines.append('«interface»' if not c['members'] and not c['bases'] else '«abstract»')
    if c['bases']:
        lines.append(', '.join(_unqualify(b) for b in c['bases']))
    lines.append(display)
    return lines


def build(classes, root_name, aggregation):
    """-> (nodes [[id, label lines]], edges [[from, to, member, 'comp' | 'aggr']])"""
    root = _resolve(classes, root_name, None)
    if root is None:
        raise SystemExit(f'class {root_name} not found in the compiled sources')
    nodes, edges, pending = {}, [], []
    instances = defaultdict(list)  # displayed type -> node ids holding one instance of it
    expanded = set()

    def expand(c, node_id):
        if node_id in expanded or (c['pure'] and c is not root):
            return
        expanded.add(node_id)
        for ty, ptr, name in c['members']:
            ty = c['aliases'].get(ty, ty)
            target = _resolve(classes, ty, c)
            if target is None or target['name'] in EXCLUDED:
                continue
            display = _unqualify(ty)
            if ptr == '':
                child = f'{display}@{node_id}.{name}'
                edges.append([node_id, child, name, 'comp'])
                add(target, display, child)
            elif aggregation:
                pending.append((node_id, target, display, name))

    def add(c, display, node_id):
        if node_id not in nodes:
            nodes[node_id] = _label(c, display)
            instances[display].append(node_id)
            expand(c, node_id)

    add(root, _unqualify(root_name), root_name)
    while pending:
        owner, target, display, name = pending.pop(0)
        held = instances.get(display, [])
        if len(held) == 1:
            dst = held[0]
        else:
            dst = display
            if dst not in nodes:
                add(target, display, dst)  # concrete classes are traced further; interfaces stop
        edges.append([owner, dst, name, 'aggr'])

    rename = {}
    for display, ids in instances.items():
        for k, nid in enumerate(ids):
            rename[nid] = display if len(ids) == 1 else f'{display} #{k + 1}'
    return ([[rename[n], lines] for n, lines in nodes.items()],
            [[rename[a], rename[b], member, kind] for a, b, member, kind in edges])


def class_graphs(files):
    """[(output name, .dot text, node count, edge count)] for every graph in GRAPHS"""
    classes = parse_classes(files)
    out = []
    for name, root, aggregation in GRAPHS:
        nodes, edges = build(classes, root, aggregation)
        title = (AGGREGATION_TITLE if aggregation else COMPOSITION_TITLE).format(root)
        out.append((name, class_graph_dot(re.sub(r'\W', '_', root), nodes, edges, title), len(nodes), len(edges)))
    return out
