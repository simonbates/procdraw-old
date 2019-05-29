import ninja_syntax
import os

class Target:
    def __init__(self, name, target_type, target_vars):
        self.name = name
        self.target_type = target_type
        self.target_vars = target_vars

class Build:
    def __init__(self):
        self.targets = []

    def add_source_set(self, name, **target_vars):
        self.targets.append(Target(name, 'source_set', target_vars))

    def add_executable(self, name, **target_vars):
        self.targets.append(Target(name, 'executable', target_vars))

def generate_ninja(build, output):
    n = ninja_syntax.Writer(output)
    n.variable('builddir', build.builddir)
    n.variable('cppflags', build.cppflags)
    n.rule('cpp', 'cl /c /showIncludes $cppflags $include_dirs /Fo$out $in',
           deps='msvc')
    for target in build.targets:
        target_build_vars = {}
        if 'include_dirs' in target.target_vars:
            include_dirs = ''
            for dir in target.target_vars['include_dirs']:
                include_dirs += '/I{:s} '.format(dir)
            target_build_vars['include_dirs'] = include_dirs.rstrip()
        for source_file in target.target_vars['sources']:
            obj_file = os.path.splitext(source_file)[0] + '.obj'
            n.build(outputs = '$builddir/' + obj_file,
                    rule = 'cpp',
                    inputs = [source_file],
                    variables = target_build_vars)
