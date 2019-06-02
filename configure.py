import os
import sys

projectdir = os.path.dirname(os.path.realpath(__file__))
sys.path.insert(0, os.path.join(projectdir, 'vendor', 'ninja'))
sys.path.insert(0, os.path.join(projectdir, 'scripts'))

from procdraw import buildgen

BUILD_DIR = 'build'
NINJA_BUILD_FILENAME = 'build.ninja'
COMPILATION_DATABASE_FILENAME = 'compile_commands.json'
GTEST_DIR = 'vendor/googletest-1.8.1/googletest'
PUGIXML_DIR = 'vendor/pugixml-1.9'

build = buildgen.Build()

build.projectdir = projectdir
build.builddir = BUILD_DIR
build.cppflags = '/nologo /EHsc'
build.cppstd = 'c++17'
build.linkflags = '/nologo'

build.add_source_set('gtest',
    sources = [
        GTEST_DIR + '/src/gtest-all.cc',
        GTEST_DIR + '/src/gtest_main.cc'
    ],
    include_dirs = [GTEST_DIR + '/include', GTEST_DIR]
)

build.add_source_set('pugixml',
    sources = [PUGIXML_DIR + '/src/pugixml.cpp']
)

build.add_source_set('procdraw_sources',
    sources = [
        'src/Color.cpp',
        'src/D3D11Graphics.cpp',
        'src/Interpreter.cpp',
        'src/Printer.cpp',
        'src/ProcdrawApp.cpp',
        'src/ProcdrawMath.cpp',
        'src/Reader.cpp',
        'src/VirtualMachine.cpp',
        'src/WinUtils.cpp'
    ],
    precompiled_header = "pch.h",
    precompiled_source = "src/pch.cpp",
    defines = ['UNICODE', '_UNICODE']
)

build.add_executable('Procdraw',
    sources = [
        'src/WinMain.cpp'
    ],
    precompiled_header = "pch.h",
    precompiled_source = "src/pch.cpp",
    defines = ['UNICODE', '_UNICODE'],
    deps = ['procdraw_sources'],
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
        'tests/PrintTo.cpp',
        'tests/ProcdrawManual.cpp',
        'tests/ProcdrawMathTests.cpp',
        'tests/TestUtils.cpp',
        'tests/VirtualMachineTests.cpp'
    ],
    precompiled_header = "pch.h",
    precompiled_source = "tests/pch.cpp",
    defines = [
        'UNICODE',
        '_UNICODE',
        'PROCDRAW_TEST_DATA_DIR=\\"tests/test_data\\"',
        'PROCDRAW_FUNCTION_DOCS_FILE=\\"Documentation/manual/function_docs.xml\\"'
    ],
    deps = ['procdraw_sources', 'gtest', 'pugixml'],
    include_dirs = ['src', GTEST_DIR + '/include', PUGIXML_DIR + '/src'],
    libs = ['d3d11.lib', 'D3DCompiler.lib']
)

buildgen.MsvcNinjaGenerator(build).write(open(NINJA_BUILD_FILENAME, 'w'))

buildgen.ClangCompilationDatabaseGenerator(build, 'clang++').write(open(COMPILATION_DATABASE_FILENAME, 'w'))
