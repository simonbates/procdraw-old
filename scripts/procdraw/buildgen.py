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
               'cl /c /showIncludes $cppflags $defines $include_dirs $pchflags /Fo$out $in',
               deps='msvc')
        n.rule('cpp_pch',
               'cl /c /showIncludes $cppflags $defines $include_dirs /Yc$pch_header /Fp$pch_pch /Fo$pch_obj $in',
               deps='msvc')
        n.rule('link', 'link $linkflags /out:$out $in $pch_obj $libs')
        for target in self.build.targets:
            self._write_target(target, n)

    def _write_target(self, target, n):
        if self._target_has_pch(target):
            self._write_pch(target, n)
        self._write_compiles(target, n)
        if target['type'] == 'executable':
            self._write_link(target, n)

    def _write_pch(self, target, n):
        pch_pch = self._get_pch_file(target)
        pch_obj = self._get_pch_obj_file(target)
        variables = self._prepare_compile_vars(target)
        variables['pch_header'] = target["precompiled_header"]
        variables['pch_pch'] = pch_pch
        variables['pch_obj'] = pch_obj
        n.build(outputs=pch_pch,
                rule='cpp_pch',
                inputs=target["precompiled_source"],
                variables=variables)

    def _write_compiles(self, target, n):
        implicit = None
        variables = self._prepare_compile_vars(target)
        if self._target_has_pch(target):
            pch_file = self._get_pch_file(target)
            implicit = pch_file
            pchflags = '/Yu' + target["precompiled_header"]
            pchflags += ' /Fp' + pch_file
            variables['pchflags'] = pchflags
        for source_file in target['sources']:
            n.build(outputs=self._get_obj_file(source_file),
                    rule='cpp',
                    inputs=source_file,
                    implicit=implicit,
                    variables=variables)

    def _prepare_compile_vars(self, target):
        variables = {}
        if 'cppflags' in target:
            variables['cppflags'] = target['cppflags']
        if 'defines' in target:
            variables['defines'] = self._format_defines(target['defines'])
        if 'include_dirs' in target:
            variables['include_dirs'] \
                = self._format_include_dirs(target['include_dirs'])
        return variables

    def _write_link(self, target, n):
        obj_files = self._get_obj_files(target)
        for dep in target['deps']:
            obj_files += self._get_obj_files(self.build.get_target(dep))
        implicit = None
        variables = {}
        if 'linkflags' in target:
            variables['linkflags'] = target['linkflags']
        if 'libs' in target:
            variables['libs'] = ' '.join(target['libs'])
        if self._target_has_pch(target):
            implicit = self._get_pch_file(target)
            variables['pch_obj'] = self._get_pch_obj_file(target)
        n.build(outputs=self._get_executable_file(target),
                rule='link',
                inputs=obj_files,
                implicit=implicit,
                variables=variables)

    def _target_has_pch(self, target):
        return 'precompiled_header' in target \
            and 'precompiled_source' in target

    def _get_obj_file(self, src_file):
        return '$builddir/' + os.path.splitext(src_file)[0] + '.obj'

    def _get_obj_files(self, target):
        return [self._get_obj_file(src_file) for src_file in target['sources']]

    def _get_pch_file(self, target):
        return '$builddir/' \
            + target['name'] + '.pch/' \
            + os.path.splitext(target['precompiled_header'])[0] + '.pch'

    def _get_pch_obj_file(self, target):
        return '$builddir/' \
            + target['name'] + '.pch/' \
            + os.path.splitext(os.path.basename(target['precompiled_source']))[0] + '.obj'

    def _get_executable_file(self, target):
        return '$builddir/' + target['name'] + '.exe'

    def _format_defines(self, defines):
        return ' '.join(['/D{:s}'.format(define) for define in defines])

    def _format_include_dirs(self, include_dirs):
        return ' '.join(['/I{:s}'.format(dir) for dir in include_dirs])
