import ninja_syntax
import os

class Build:
    def __init__(self):
        self.targets = []

    def add_source_set(self, name, **target_vars):
        self.__add_target(name, 'source_set', target_vars)

    def add_executable(self, name, **target_vars):
        self.__add_target(name, 'executable', target_vars)

    def get_target(self, name):
        for target in self.targets:
            if target['name'] == name:
                return target

    def __add_target(self, name, type, target_vars):
        target = {'name': name, 'type': type}
        target.update(target_vars)
        self.targets.append(target)

def generate_ninja(build, output):
    n = ninja_syntax.Writer(output)
    n.variable('builddir', build.builddir)
    n.variable('cppflags', build.cppflags)
    n.variable('linkflags', build.linkflags)
    n.rule('cpp', 'cl /c /showIncludes $cppflags $include_dirs /Fo$out $in',
           deps='msvc')
    n.rule('link', 'link $linkflags /out:$out $in $libs')
    for target in build.targets:
        write_target(build, target, n)

def write_target(build, target, n):
    write_compiles(target, n)
    if target['type'] == 'executable':
        write_link(build, target, n)

def write_compiles(target, n):
    variables = {}
    if 'include_dirs' in target:
        variables['include_dirs'] = format_include_dirs(target['include_dirs'])
    for source_file in target['sources']:
        n.build(outputs = get_obj_file(source_file),
                rule = 'cpp',
                inputs = source_file,
                variables = variables)

def write_link(build, target, n):
    obj_files = get_obj_files(target)
    for dep in target['deps']:
        obj_files = obj_files + get_obj_files(build.get_target(dep))
    variables = {}
    if 'libs' in target:
        variables['libs'] = ' '.join(target['libs'])
    n.build(outputs = get_executable_file(target),
            rule = 'link',
            inputs = obj_files,
            variables = variables)

def get_obj_file(src_file):
    return '$builddir/' + os.path.splitext(src_file)[0] + '.obj'

def get_obj_files(target):
    obj_files = []
    for source_file in target['sources']:
        obj_files.append(get_obj_file(source_file))
    return obj_files

def get_executable_file(target):
    return '$builddir/' + target['name'] + '.exe'

def format_include_dirs(include_dirs):
    formatted = ''
    for dir in include_dirs:
        formatted += '/I{:s} '.format(dir)
    return formatted.rstrip()
