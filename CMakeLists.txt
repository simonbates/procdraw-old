cmake_minimum_required(VERSION 3.15)

project(Procdraw)

enable_testing()

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(UNICODE _UNICODE PLOG_OMIT_LOG_DEFINES)

# Dependencies

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_path(PLOG_INCLUDE_DIR plog/Log.h)

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

# Test support

add_library(procdraw_test_support
        src/test_support/SourceFileChecker.cpp
        src/test_support/TapReporter.cpp)

# Unit tests

add_executable(procdraw_unit_tests
        src/unit_tests/ColourTest.cpp
        src/unit_tests/FunctionsTest.cpp
        src/unit_tests/InterpreterPrintTest.cpp
        src/unit_tests/InterpreterReadTest.cpp
        src/unit_tests/InterpreterTest.cpp
        src/unit_tests/InterpreterTypesTest.cpp
        src/unit_tests/ProcdrawMathTest.cpp
        src/unit_tests/SimpleMovingAverageTest.cpp
        src/unit_tests/SourceFileCheckerTest.cpp)

target_link_libraries(procdraw_unit_tests
        procdraw_lib
        procdraw_test_support
        GTest::gtest
        GTest::gtest_main)

# check_cpp_files

add_executable(check_cpp_files
        src/check_cpp_files/main.cpp)

target_link_libraries(check_cpp_files
        procdraw_test_support)

# Register CTest tests

file(TO_NATIVE_PATH "../src" PROCDRAW_SRC_DIR)

add_test(NAME procdraw_unit_tests
        COMMAND procdraw_unit_tests)

add_test(NAME check_cpp_files
        COMMAND check_cpp_files 42 "${PROCDRAW_SRC_DIR}")
