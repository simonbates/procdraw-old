import os
import subprocess
import sys

from lxml import etree


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


class Apache2HeaderChecker:
    def __init__(self):
        self.file = None
        self.prefix = None
        self.line = None

    def check(self, file, prefix):
        self.file = file
        self.prefix = prefix
        with open(file) as file_in:
            self.line = file_in.readline()
            if not self._check_copyright(file_in):
                return False
            if self._check_header(file_in):
                print("ok {:s}".format(file))
                return True
            else:
                return False

    def _check_copyright(self, file_in):
        if not self.line.startswith(self.prefix + " Copyright "):
            self._not_ok(self.prefix + " Copyright \n", self.line)
            return False
        self.line = file_in.readline()
        while self.line.startswith(self.prefix + " Copyright "):
            self.line = file_in.readline()
        return True

    def _check_header(self, file_in):
        for i in range(len(_apache2_header)):
            if self.line != self.prefix + _apache2_header[i]:
                self._not_ok(self.prefix + _apache2_header[i], self.line)
                return False
            else:
                self.line = file_in.readline()
        return True

    def _not_ok(self, expected, got):
        print("not ok {:s}".format(self.file))
        print("Expected:")
        print(expected, end="")
        print("Got:")
        print(got, end="")


def find(paths):
    for path in paths:
        for (dirpath, dirnames, filenames) in os.walk(path):
            for filename in filenames:
                yield os.path.join(dirpath, filename)


def is_cpp_file(filename):
    return filename.endswith(".cpp") or filename.endswith(".h")


def find_cpp_files(paths):
    return (f for f in find(paths) if is_cpp_file(f))


def mkdir_p(path):
    if not os.path.exists(path):
        os.makedirs(os.path.abspath(path))


def run(args):
    args = list(args)
    print(" ".join(args))
    completed_process = subprocess.run(args, shell=False)
    if completed_process.returncode != 0:
        sys.exit("*** ERROR {:d}".format(completed_process.returncode))


def check_file_headers(files):
    at_least_one_fail = False
    checker = Apache2HeaderChecker()
    for file in files:
        if not checker.check(file, "//"):
            at_least_one_fail = True
    if at_least_one_fail:
        sys.exit(1)


def validate_xml(schema_file, xml_file):
    schema_doc = etree.parse(schema_file)
    validator = etree.RelaxNG(schema_doc)

    xml_doc = etree.parse(xml_file)

    if validator.validate(xml_doc):
        print("ok {:s}".format(os.path.relpath(xml_file)))
    else:
        sys.exit(validator.error_log)
