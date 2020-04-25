import io
import unittest
import procdraw_utils


_good_cpp = '\n'.join([
    '// Copyright 2020 Some Name',
    '//',
    '// Licensed under the Apache License, Version 2.0 (the "License");',
    '// you may not use this file except in compliance with the License.',
    '// You may obtain a copy of the License at',
    '//',
    '//     http://www.apache.org/licenses/LICENSE-2.0',
    '//',
    '// Unless required by applicable law or agreed to in writing, software',
    '// distributed under the License is distributed on an "AS IS" BASIS,',
    '// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.',  # noqa
    '// See the License for the specific language governing permissions and',
    '// limitations under the License.',
    '',
    'namespace Procdraw {',
    '}'])

_good_cpp_multiple_copyright = '\n'.join([
    '// Copyright 2020 Some Name1',
    '// Copyright 2020 Some Name2',
    '// Copyright 2020 Some Name3',
    '//',
    '// Licensed under the Apache License, Version 2.0 (the "License");',
    '// you may not use this file except in compliance with the License.',
    '// You may obtain a copy of the License at',
    '//',
    '//     http://www.apache.org/licenses/LICENSE-2.0',
    '//',
    '// Unless required by applicable law or agreed to in writing, software',
    '// distributed under the License is distributed on an "AS IS" BASIS,',
    '// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.',  # noqa
    '// See the License for the specific language governing permissions and',
    '// limitations under the License.',
    '',
    'namespace Procdraw {',
    '}'])

_good_header = '\n'.join([
    '// Copyright 2020 Some Name',
    '//',
    '// Licensed under the Apache License, Version 2.0 (the "License");',
    '// you may not use this file except in compliance with the License.',
    '// You may obtain a copy of the License at',
    '//',
    '//     http://www.apache.org/licenses/LICENSE-2.0',
    '//',
    '// Unless required by applicable law or agreed to in writing, software',
    '// distributed under the License is distributed on an "AS IS" BASIS,',
    '// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.',  # noqa
    '// See the License for the specific language governing permissions and',
    '// limitations under the License.',
    '',
    '#pragma once',
    '',
    'namespace Procdraw {',
    '}'])


class TestSourceFileChecker(unittest.TestCase):
    def setUp(self):
        self.checker = procdraw_utils.SourceFileChecker()

    def test_empty(self):
        empty = io.StringIO("")
        result = self.checker.check("empty.cpp", empty, "//")
        self.assertFalse(result.is_ok)
        self.assertEqual("empty.cpp", result.description)
        self.assertEqual({
            "expected": "// Copyright ...",
            "got": ""
            }, result.details)

    def test_no_copyright(self):
        no_copyright = io.StringIO("asdf")
        result = self.checker.check("no_copyright.cpp", no_copyright, "//")
        self.assertFalse(result.is_ok)
        self.assertEqual("no_copyright.cpp", result.description)
        self.assertEqual({
            "expected": "// Copyright ...",
            "got": "asdf"
            }, result.details)

    def test_bad_apache2_header(self):
        bad_header = io.StringIO('\n'.join([
          '// Copyright 2020 Some Name',
          '//',
          '// Licensed under the Apache License, Version 2.0 (the "License");',
          '// you may not use this file except in compliance with the License.',  # noqa
          '// asdf']))
        result = self.checker.check("bad_header.cpp", bad_header, "//")
        self.assertFalse(result.is_ok)
        self.assertEqual("bad_header.cpp", result.description)
        self.assertEqual({
            "expected": "// You may obtain a copy of the License at",
            "got": "// asdf"
            }, result.details)

    def test_missing_pragma_once(self):
        missing_once = io.StringIO(_good_cpp)
        result = self.checker.check("missing_once.h", missing_once, "//")
        self.assertFalse(result.is_ok)
        self.assertEqual("missing_once.h", result.description)
        self.assertEqual({
            "expected": "#pragma once",
            "got": "namespace Procdraw {"
            }, result.details)

    def test_good_cpp(self):
        good_cpp = io.StringIO(_good_cpp)
        result = self.checker.check("good_cpp.cpp", good_cpp, "//")
        self.assertTrue(result.is_ok)
        self.assertEqual("good_cpp.cpp", result.description)
        self.assertIsNone(result.details)

    def test_multiple_copyright(self):
        multiple_copyright = io.StringIO(_good_cpp_multiple_copyright)
        result = self.checker.check("multiple_copyright.cpp",
                                    multiple_copyright, "//")
        self.assertTrue(result.is_ok)
        self.assertEqual("multiple_copyright.cpp", result.description)
        self.assertIsNone(result.details)

    def test_good_header(self):
        good_header = io.StringIO(_good_header)
        result = self.checker.check("good_header.h", good_header, "//")
        self.assertTrue(result.is_ok)
        self.assertEqual("good_header.h", result.description)
        self.assertIsNone(result.details)


if __name__ == '__main__':
    unittest.main()
