import json
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
            if target.name == name:
                return target

    def _add_target(self, name, type, target_vars):
        self.targets.append(Target(name, type, target_vars))

class Target:
    def __init__(self, name, type, vars):
        self.name = name
        self.type = type
        self.sources = vars.get('sources', [])
        self.precompiled_header = vars.get('precompiled_header', None)
        self.precompiled_source = vars.get('precompiled_source', None)
        self.cppflags = vars.get('cppflags', None)
        self.defines = vars.get('defines', [])
        self.deps = vars.get('deps', [])
        self.include_dirs = vars.get('include_dirs', [])
        self.libs = vars.get('libs', [])

    def has_pch(self):
        return ((self.precompiled_header is not None) and
            (self.precompiled_source is not None))

class MsvcNinjaGenerator:
    def __init__(self, build):
        self.build = build

    def write(self, output):
        n = ninja_syntax.Writer(output)
        n.variable('builddir', self.build.builddir)
        n.variable('cppflags', self.build.cppflags)
        n.variable('cppstd', self.build.cppstd)
        n.variable('linkflags', self.build.linkflags)
        n.rule('cpp',
               'cl /c /showIncludes $cppflags /std:$cppstd $defines $include_dirs $pchflags /Fo$out $in',
               deps='msvc')
        n.rule('cpp_pch',
               'cl /c /showIncludes $cppflags /std:$cppstd $defines $include_dirs /Yc$pch_header /Fp$pch_pch /Fo$pch_obj $in',
               deps='msvc')
        n.rule('link', 'link $linkflags /out:$out $in $pch_objs $libs')
        for target in self.build.targets:
            self._write_target(target, n)

    def _write_target(self, target, n):
        if target.has_pch():
            self._write_pch(target, n)
        self._write_compiles(target, n)
        if target.type == 'executable':
            self._write_link(target, n)

    def _write_pch(self, target, n):
        pch_pch_file = self._get_pch_pch_file(target)
        pch_obj_file = self._get_pch_obj_file(target)
        variables = self._prepare_compile_vars(target)
        variables['pch_header'] = target.precompiled_header
        variables['pch_pch'] = pch_pch_file
        variables['pch_obj'] = pch_obj_file
        n.build(outputs=pch_pch_file,
                rule='cpp_pch',
                inputs=target.precompiled_source,
                variables=variables)

    def _write_compiles(self, target, n):
        implicit = []
        variables = self._prepare_compile_vars(target)
        if target.has_pch():
            pch_pch_file = self._get_pch_pch_file(target)
            implicit.append(pch_pch_file)
            variables['pchflags'] = '/Yu{:s} /Fp{:s}'.format(
                target.precompiled_header, pch_pch_file)
        for source_file in target.sources:
            n.build(outputs=self._get_obj_file(source_file),
                    rule='cpp',
                    inputs=source_file,
                    implicit=implicit,
                    variables=variables)

    def _prepare_compile_vars(self, target):
        return {
            'cppflags': target.cppflags,
            'defines': self._format_defines(target.defines),
            'include_dirs': self._format_include_dirs(target.include_dirs)
        }

    def _write_link(self, target, n):
        obj_files = self._get_obj_files(target)
        pch_obj_files = []
        implicit = []
        if target.has_pch():
            pch_obj_files.append(self._get_pch_obj_file(target))
            implicit.append(self._get_pch_pch_file(target))
        for dep in target.deps:
            dep_target = self.build.get_target(dep)
            obj_files += self._get_obj_files(dep_target)
            if dep_target.has_pch():
                pch_obj_files.append(self._get_pch_obj_file(dep_target))
        variables = {
            'libs': ' '.join(target.libs),
            'pch_objs': ' '.join(pch_obj_files)
        }
        n.build(outputs=self._get_executable_file(target),
                rule='link',
                inputs=obj_files,
                implicit=implicit,
                variables=variables)

    def _get_obj_file(self, src_file):
        return '$builddir/{:s}.obj'.format(os.path.splitext(src_file)[0])

    def _get_obj_files(self, target):
        return [self._get_obj_file(src_file) for src_file in target.sources]

    def _get_pch_pch_file(self, target):
        return '$builddir/{:s}.pch/{:s}.pch'.format(target.name,
            os.path.splitext(os.path.basename(target.precompiled_header))[0])

    def _get_pch_obj_file(self, target):
        return '$builddir/{:s}.pch/{:s}.obj'.format(target.name,
            os.path.splitext(os.path.basename(target.precompiled_source))[0])

    def _get_executable_file(self, target):
        return '$builddir/{:s}.exe'.format(target.name)

    def _format_defines(self, defines):
        if len(defines) > 0:
            return ' '.join(['/D{:s}'.format(define) for define in defines])
        else:
            return None

    def _format_include_dirs(self, include_dirs):
        if len(include_dirs) > 0:
            return ' '.join(['/I{:s}'.format(dir) for dir in include_dirs])
        else:
            return None

class ClangCompilationDatabaseGenerator:
    def __init__(self, build, command_prog):
        self.build = build
        self.command_prog = command_prog

    def write(self, output):
        db = []
        for target in self.build.targets:
            db.extend(self._get_command_objects(target))
        print(json.dumps(db, indent=4), file=output)

    def _get_command_objects(self, target):
        command_objects = []
        for source_file in target.sources:
            command = [self.command_prog]
            command.append('-std={:s}'.format(self.build.cppstd))
            for define in target.defines:
                command.append('-D{:s}'.format(define))
            for dir in target.include_dirs:
                command.append('-I{:s}'.format(dir))
            command_objects.append({
                'directory': self.build.projectdir,
                'command': ' '.join(command),
                'file': source_file
            })
        return command_objects
