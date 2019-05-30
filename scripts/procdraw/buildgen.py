import ninja_syntax
import os

class Build:
    def __init__(self):
        self.targets = []

    def add_source_set(self, name, **target_vars):
        self._add_target(name, 'source_set', target_vars)

    def add_executable(self, name, **target_vars):
        self._add_target(name, 'executable', target_vars)

    def get_target(self, name):
        for target in self.targets:
            if target['name'] == name:
                return target

    def _add_target(self, name, type, target_vars):
        target = {'name': name, 'type': type}
        target.update(target_vars)
        self.targets.append(target)

class Generator:
    def __init__(self, build):
        self.build = build

    def write(self, output):
        n = ninja_syntax.Writer(output)
        n.variable('builddir', self.build.builddir)
        n.variable('cppflags', self.build.cppflags)
        n.variable('linkflags', self.build.linkflags)
        n.rule('cpp',
               'cl /c /showIncludes $cppflags $defines $include_dirs /Fo$out $in',
               deps='msvc')
        n.rule('link', 'link $linkflags /out:$out $in $libs')
        for target in self.build.targets:
            self._write_target(target, n)

    def _write_target(self, target, n):
        self._write_compiles(target, n)
        if target['type'] == 'executable':
            self._write_link(target, n)

    def _write_compiles(self, target, n):
        variables = {}
        if 'cppflags' in target:
            variables['cppflags'] = target['cppflags']
        if 'defines' in target:
            variables['defines'] = self._format_defines(target['defines'])
        if 'include_dirs' in target:
            variables['include_dirs'] \
                = self._format_include_dirs(target['include_dirs'])
        for source_file in target['sources']:
            n.build(outputs=self._get_obj_file(source_file),
                    rule='cpp',
                    inputs=source_file,
                    variables=variables)

    def _write_link(self, target, n):
        obj_files = self._get_obj_files(target)
        for dep in target['deps']:
            obj_files += self._get_obj_files(self.build.get_target(dep))
        variables = {}
        if 'linkflags' in target:
            variables['linkflags'] = target['linkflags']
        if 'libs' in target:
            variables['libs'] = ' '.join(target['libs'])
        n.build(outputs=self._get_executable_file(target),
                rule='link',
                inputs=obj_files,
                variables=variables)

    def _get_obj_file(self, src_file):
        return '$builddir/' + os.path.splitext(src_file)[0] + '.obj'

    def _get_obj_files(self, target):
        return [self._get_obj_file(src_file) for src_file in target['sources']]

    def _get_executable_file(self, target):
        return '$builddir/' + target['name'] + '.exe'

    def _format_defines(self, defines):
        return ' '.join(['/D{:s}'.format(define) for define in defines])

    def _format_include_dirs(self, include_dirs):
        return ' '.join(['/I{:s}'.format(dir) for dir in include_dirs])
