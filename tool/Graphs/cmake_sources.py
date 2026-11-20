"""The source files that CMake compiles for one configure preset, found by following the
add_subdirectory() / target_sources() calls from the root CMakeLists.txt.

- Files commented out in target_sources() (e.g. `#        Sensor_STTS22H.cpp`) are left out.
- Headers in an active directory count even when not listed: they can still be #included.
- Only files under src/ count (src_3rd/ and the like are out of scope).
"""
import json
import os
import re


def preset_variables(root, preset):
    """cacheVariables of a configure preset in CMakePresets.json (APP, TARGET, BOARD, PLATFORM, ...)"""
    with open(os.path.join(root, 'CMakePresets.json'), encoding='utf-8') as f:
        presets = {p['name']: p for p in json.load(f)['configurePresets']}
    return {k: str(v) for k, v in presets[preset].get('cacheVariables', {}).items()}


def active_files(root, variables):
    """{directory: set of absolute file paths} for every directory with active files"""
    root = os.path.normpath(root)
    src = os.path.join(root, 'src')

    def subst(s, cur):
        s = s.replace('${SOURCE_ROOT_DIR}', src).replace('${CMAKE_SOURCE_DIR}', root)
        for k, v in variables.items():
            s = s.replace('${%s}' % k, v)
        return os.path.normpath(s if os.path.isabs(s) else os.path.join(cur, s))

    active = {}

    def walk(d):
        if not (d + os.sep).startswith(src + os.sep):
            return
        with open(os.path.join(d, 'CMakeLists.txt'), encoding='utf-8', errors='replace') as f:
            txt = f.read()
        listed, commented = set(), set()
        for line in txt.splitlines():
            s = line.strip()
            m = re.fullmatch(r'#\s*([\w./-]+\.(?:h|hpp|c|cpp|cppm|ixx))\s*', s)  # a file that is commented out
            if m:
                commented.add(os.path.normpath(os.path.join(d, m.group(1))))
                continue
            if s.startswith('#'):
                continue
            for tok in re.findall(r'([\w./${}-]+\.(?:hpp|h|cppm|cpp|c|ixx))\b', s):
                listed.add(subst(tok, d))
        files = {f for f in listed if os.path.exists(f) and f.startswith(src + os.sep)}
        for fn in os.listdir(d):
            p = os.path.normpath(os.path.join(d, fn))
            if os.path.isfile(p) and fn.endswith(('.h', '.hpp')) and p not in commented:
                files.add(p)
        files -= commented
        for f in files:
            active.setdefault(os.path.dirname(f), set()).add(f)
        for m in re.finditer(r'^\s*add_subdirectory\(\s*([^\s)]+)', txt, re.M):
            walk(subst(m.group(1), d))

    with open(os.path.join(root, 'CMakeLists.txt'), encoding='utf-8', errors='replace') as f:
        top = f.read()
    for m in re.finditer(r'^\s*add_subdirectory\(\s*([^\s)]+)', top, re.M):
        walk(subst(m.group(1), root))
    return {d: fs for d, fs in active.items() if fs}
