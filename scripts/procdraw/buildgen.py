import ninja_syntax
import os

class Build:
    def __init__(self):
        self.targets = []

    def add_source_set(self, name, **target_vars):
        target = {'name': name, 'type': 'source_set'}
        target.update(target_vars)
        self.targets.append(target)

    def add_executable(self, name, **target_vars):
        target = {'name': name, 'type': 'executable'}
        target.update(target_vars)
        self.targets.append(target)

def generate_ninja(build, output):
    n = ninja_syntax.Writer(output)
    n.variable('builddir', build.builddir)
    n.variable('cppflags', build.cppflags)
    n.rule('cpp', 'cl /c /showIncludes $cppflags $include_dirs /Fo$out $in',
           deps='msvc')
    for target in build.targets:
        write_target(target, n)

def write_target(target, n):
    target_build_vars = {}
    if 'include_dirs' in target:
        target_build_vars['include_dirs'] \
            = format_include_dirs(target['include_dirs'])
    for source_file in target['sources']:
        n.build(outputs = obj_file(source_file),
                rule = 'cpp',
                inputs = source_file,
                variables = target_build_vars)

def obj_file(src_file):
    return '$builddir/' + os.path.splitext(src_file)[0] + '.obj'

def format_include_dirs(include_dirs):
    formatted = ''
    for dir in include_dirs:
        formatted += '/I{:s} '.format(dir)
    return formatted.rstrip()
