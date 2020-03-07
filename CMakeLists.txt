cmake_minimum_required(VERSION 3.15)

project(Procdraw)

enable_testing()

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(UNICODE _UNICODE PLOG_OMIT_LOG_DEFINES)

# Dependencies

find_package(Catch2 REQUIRED)
find_path(PLOG_INCLUDE_DIR plog/Log.h)
find_package(pugixml REQUIRED)
find_path(WIL_INCLUDE_DIR wil/com.h)

# procdraw_lib

add_library(procdraw_lib
        src/lib/Colour.cpp
        src/lib/D3D11Graphics.cpp
        src/lib/Interpreter.cpp
        src/lib/Printer.cpp
        src/lib/ProcdrawApp.cpp
        src/lib/ProcdrawMath.cpp
        src/lib/Reader.cpp
        src/lib/WinUtils.cpp)

target_include_directories(procdraw_lib
        PUBLIC ${WIL_INCLUDE_DIR})

# procdraw executable

add_executable(procdraw WIN32
        src/winmain/WinMain.cpp)

target_link_libraries(procdraw
        procdraw_lib
        d3d11.lib
        D3DCompiler.lib)

target_include_directories(procdraw
        PRIVATE ${PLOG_INCLUDE_DIR})

# Tests

get_filename_component(PROCDRAW_TEST_DATA_DIR "${PROJECT_SOURCE_DIR}/src/tests/test_data" ABSOLUTE)
get_filename_component(PROCDRAW_DOCS_FILE "${PROJECT_SOURCE_DIR}/docs/docs.xml" ABSOLUTE)

add_definitions(-DPROCDRAW_TEST_DATA_DIR="${PROCDRAW_TEST_DATA_DIR}")
add_definitions(-DPROCDRAW_DOCS_FILE="${PROCDRAW_DOCS_FILE}")

add_executable(procdraw_tests
        src/tests/ColourTests.cpp
        src/tests/DocsTester.cpp
        src/tests/DocsTesterTests.cpp
        src/tests/FunctionDocsTests.cpp
        src/tests/InterpreterReadTests.cpp
        src/tests/InterpreterPrintTests.cpp
        src/tests/InterpreterTests.cpp
        src/tests/InterpreterTypesTests.cpp
        src/tests/ProcdrawDocs.cpp
        src/tests/ProcdrawMathTests.cpp
        src/tests/TestsMain.cpp)

target_link_libraries(procdraw_tests
        procdraw_lib
        Catch2::Catch2
        pugixml)

# Register CTest tests

add_test(NAME validate-xml
        COMMAND invoke validate-xml)

add_test(NAME check-file-headers
        COMMAND invoke check-file-headers)

add_test(NAME procdraw_tests
        COMMAND procdraw_tests)
