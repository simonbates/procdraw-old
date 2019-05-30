import os
import sys

projectdir = os.path.dirname(os.path.realpath(__file__))
sys.path.insert(0, os.path.join(projectdir, 'vendor', 'ninja'))
sys.path.insert(0, os.path.join(projectdir, 'scripts'))

from procdraw import buildgen

BUILD_DIR = 'build'
BUILD_FILENAME = 'build.ninja'
GTEST_DIR = 'vendor/googletest-1.8.1/googletest'
PUGIXML_DIR = 'vendor/pugixml-1.9'

build = buildgen.Build()

build.builddir = BUILD_DIR
build.cppflags = '/nologo /EHsc /DUNICODE /D_UNICODE'
build.linkflags = '/nologo'

build.add_source_set('gtest',
    sources = [
        GTEST_DIR + '/src/gtest-all.cc',
        GTEST_DIR + '/src/gtest_main.cc'
    ],
    cppflags = '/nologo /EHsc',
    include_dirs = [GTEST_DIR + '/include', GTEST_DIR]
)

build.add_source_set('pugixml',
    sources = [PUGIXML_DIR + '/src/pugixml.cpp'],
    cppflags = '/nologo /EHsc'
)

build.add_source_set('procdraw_sources',
    sources = [
        'src/Color.cpp',
        'src/Interpreter.cpp',
        'src/Printer.cpp',
        'src/ProcdrawMath.cpp',
        'src/Reader.cpp',
        'src/VirtualMachine.cpp',
    ]
)

build.add_executable('Procdraw',
    sources = [
        'src/D3D11Graphics.cpp',
        'src/ProcdrawApp.cpp',
        'src/WinUtils.cpp',
        'winmain/WinMain.cpp'
    ],
    deps = ['procdraw_sources'],
    include_dirs = ['src'],
    libs = ['d3d11.lib', 'D3DCompiler.lib']
)

build.add_executable('procdraw_tests',
    sources = [
        'tests/ColorTests.cpp',
        'tests/DocTester.cpp',
        'tests/DocTesterTests.cpp',
        'tests/FunctionDocsTests.cpp',
        'tests/InterpreterPrintTests.cpp',
        'tests/InterpreterReadTests.cpp',
        'tests/InterpreterTests.cpp',
        'tests/pch.cpp',
        'tests/PrintTo.cpp',
        'tests/ProcdrawManual.cpp',
        'tests/ProcdrawMathTests.cpp',
        'tests/TestUtils.cpp',
        'tests/VirtualMachineTests.cpp'
    ],
    defines = [
        'PROCDRAW_TEST_DATA_DIR=\\"tests/test_data\\"',
        'PROCDRAW_FUNCTION_DOCS_FILE=\\"Documentation/manual/function_docs.xml\\"'
    ],
    deps = ['procdraw_sources', 'gtest', 'pugixml'],
    include_dirs = ['src', GTEST_DIR + '/include', PUGIXML_DIR + '/src']
)

buildgen.generate_ninja(build, open(BUILD_FILENAME, 'w'))
