cmake_minimum_required(VERSION 3.15)

project(Procdraw)

enable_testing()

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(UNICODE _UNICODE PLOG_OMIT_LOG_DEFINES)

# Dependencies

find_package(Catch2 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_path(PLOG_INCLUDE_DIR plog/Log.h)
find_package(pugixml REQUIRED)

# procdraw_lib

add_library(procdraw_lib
        src/lib/glrenderer/Camera.cpp
        src/lib/glrenderer/Cube.cpp
        src/lib/glrenderer/FlatProgram.cpp
        src/lib/glrenderer/GLRenderer.cpp
        src/lib/glrenderer/GLUtils.cpp
        src/lib/Colour.cpp
        src/lib/FrameTimer.cpp
        src/lib/Interpreter.cpp
        src/lib/Printer.cpp
        src/lib/ProcdrawApp.cpp
        src/lib/ProcdrawMath.cpp
        src/lib/Reader.cpp
        GL/src/glad.c)

target_include_directories(procdraw_lib
        PUBLIC GL/include
        PRIVATE ${PLOG_INCLUDE_DIR})

# procdraw executable

add_executable(procdraw WIN32
        src/winmain/WinMain.cpp)

target_link_libraries(procdraw
        procdraw_lib
        glfw
        glm)

target_include_directories(procdraw
        PRIVATE ${PLOG_INCLUDE_DIR})

# Unit tests

add_executable(procdraw_unit_tests
        src/unit_tests/ColourTest.cpp
        src/unit_tests/InterpreterPrintTest.cpp
        src/unit_tests/InterpreterReadTest.cpp
        src/unit_tests/InterpreterTest.cpp
        src/unit_tests/InterpreterTypesTest.cpp
        src/unit_tests/ProcdrawMathTest.cpp
        src/unit_tests/SimpleMovingAverageTest.cpp)

target_link_libraries(procdraw_unit_tests
        procdraw_lib
        GTest::gtest
        GTest::gtest_main)

# Tests

get_filename_component(PROCDRAW_TEST_DATA_DIR "${PROJECT_SOURCE_DIR}/src/tests/test_data" ABSOLUTE)
get_filename_component(PROCDRAW_DOCS_FILE "${PROJECT_SOURCE_DIR}/docs/docs.xml" ABSOLUTE)

add_definitions(-DPROCDRAW_TEST_DATA_DIR="${PROCDRAW_TEST_DATA_DIR}")
add_definitions(-DPROCDRAW_DOCS_FILE="${PROCDRAW_DOCS_FILE}")

add_executable(procdraw_tests
        src/tests/DocsTester.cpp
        src/tests/DocsTesterTests.cpp
        src/tests/FunctionDocsTests.cpp
        src/tests/ProcdrawDocs.cpp
        src/tests/TestsMain.cpp)

target_link_libraries(procdraw_tests
        procdraw_lib
        Catch2::Catch2
        pugixml)

# Register CTest tests

add_test(NAME procdraw_unit_tests
        COMMAND procdraw_unit_tests)

add_test(NAME procdraw_tests
        COMMAND procdraw_tests)
