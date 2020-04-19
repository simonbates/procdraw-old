import os
import subprocess
import sys

from lxml import etree
import yaml


_apache2_header = [
 '\n',
 ' Licensed under the Apache License, Version 2.0 (the "License");\n',
 ' you may not use this file except in compliance with the License.\n',
 ' You may obtain a copy of the License at\n',
 '\n',
 '     http://www.apache.org/licenses/LICENSE-2.0\n',
 '\n',
 ' Unless required by applicable law or agreed to in writing, software\n',
 ' distributed under the License is distributed on an "AS IS" BASIS,\n',
 ' WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n',
 ' See the License for the specific language governing permissions and\n',
 ' limitations under the License.\n'
]


class CheckResult:
    def __init__(self, is_ok, description, details=None):
        self.is_ok = is_ok
        self.description = description
        self.details = details


class CheckResultTapReporter:
    def __init__(self, plan):
        self.plan = plan
        self.num_tests = 0
        self.num_pass = 0
        self.num_fail = 0
        print("1..{:d}".format(self.plan))

    def add(self, result):
        self.num_tests += 1
        if result.is_ok:
            print("ok {:d} {:s}".format(self.num_tests, result.description))
            self.num_pass += 1
        else:
            print("not ok {:d} {:s}".format(self.num_tests,
                                            result.description))
            self.num_fail += 1
        if result.details is not None:
            print("  ---")
            for line in yaml.dump(result.details).splitlines():
                print("  {}".format(line))
            print("  ---")

    def is_fail(self):
        return (self.num_fail > 0) or (self.num_tests != self.plan)

    def print_summary(self):
        print("# tests {:d}".format(self.num_tests))
        print("# pass  {:d}".format(self.num_pass))
        if self.num_fail > 0:
            print("# fail  {:d}".format(self.num_fail))
        if self.num_tests != self.plan:
            print("# unexpected number of tests")
        if not self.is_fail():
            print("# ok")


class SourceFileChecker:
    def __init__(self):
        self.filename = None
        self.file_in = None
        self.prefix = None
        self.line = None

    def check(self, filename, file_in, prefix):
        self.filename = filename
        self.file_in = file_in
        self.prefix = prefix
        checks = []
        checks.append(self._check_copyright)
        checks.append(self._check_apache2_header)
        if is_cpp_header(filename):
            checks.append(self._check_pragma_once)
        self._read_next_line()
        for check in checks:
            is_ok, result = check()
            if not is_ok:
                return result
        return self._ok()

    def _read_next_line(self):
        self.line = self.file_in.readline()

    def _check_copyright(self):
        if not self.line.startswith(self.prefix + " Copyright "):
            return False, self._not_ok(self.prefix + " Copyright ...")
        self._read_next_line()
        while self.line.startswith(self.prefix + " Copyright "):
            self._read_next_line()
        return True, None

    def _check_apache2_header(self):
        for i in range(len(_apache2_header)):
            if self.line != self.prefix + _apache2_header[i]:
                return False, self._not_ok(self.prefix + _apache2_header[i])
            self._read_next_line()
        return True, None

    def _check_pragma_once(self):
        while len(self.line.strip()) == 0:
            self._read_next_line()
        if self.line != "#pragma once\n":
            return False, self._not_ok("#pragma once")
        else:
            return True, None

    def _ok(self):
        return CheckResult(True, self.filename)

    def _not_ok(self, expected):
        return CheckResult(False, self.filename, {
            "expected": expected.rstrip(),
            "got": self.line.rstrip()
        })


def find(paths):
    for path in paths:
        for (dirpath, dirnames, filenames) in os.walk(path):
            for filename in filenames:
                yield os.path.join(dirpath, filename)


def is_cpp_file(filename):
    return filename.endswith(".cpp") or filename.endswith(".h")


def is_cpp_header(filename):
    return is_cpp_file(filename) and not filename.endswith(".cpp")


def find_cpp_files(paths):
    return (f for f in find(paths) if is_cpp_file(f))


def is_python_file(filename):
    return filename.endswith(".py")


def find_python_files(paths):
    return (f for f in find(paths) if is_python_file(f))


def mkdir_p(path):
    if not os.path.exists(path):
        os.makedirs(os.path.abspath(path))


def run(args):
    args = list(args)
    print(" ".join(args))
    completed_process = subprocess.run(args, shell=False)
    if completed_process.returncode != 0:
        sys.exit("*** ERROR {:d}".format(completed_process.returncode))


def validate_xml(schema_file, xml_file):
    schema_doc = etree.parse(schema_file)
    validator = etree.RelaxNG(schema_doc)
    xml_doc = etree.parse(xml_file)
    if validator.validate(xml_doc):
        return CheckResult(True, os.path.relpath(xml_file))
    else:
        errors = []
        for error in validator.error_log:
            errors.append("{:d}:{:d}: {:s}".format(error.line, error.column,
                                                   error.message))
        return CheckResult(False, os.path.relpath(xml_file), {
            "errors": errors
        })
