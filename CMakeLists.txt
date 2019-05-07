cmake_minimum_required(VERSION 3.12)

project(procdraw)

# Google Test

set(GTEST_DIR ${PROJECT_SOURCE_DIR}/vendor/googletest-1.8.1/googletest)

add_library(gtest ${GTEST_DIR}/src/gtest-all.cc)
target_include_directories(gtest
        PUBLIC ${GTEST_DIR}/include
        PRIVATE ${GTEST_DIR})

add_library(gtest_main ${GTEST_DIR}/src/gtest_main.cc)
target_include_directories(gtest_main PUBLIC ${GTEST_DIR}/include)

# procdraw_lib

add_library(procdraw_lib
        ProcdrawLib/Color.cpp
        ProcdrawLib/D3D11Graphics.cpp
        ProcdrawLib/Interpreter.cpp
        ProcdrawLib/Printer.cpp
        ProcdrawLib/ProcdrawApp.cpp
        ProcdrawLib/ProcdrawMath.cpp
        ProcdrawLib/Reader.cpp
        ProcdrawLib/VirtualMachine.cpp
        ProcdrawLib/WinUtils.cpp)

target_compile_definitions(procdraw_lib PRIVATE UNICODE _UNICODE)

target_include_directories(procdraw_lib PUBLIC ProcdrawLib)

# procdraw executable

add_executable(procdraw WIN32 Procdraw/WinMain.cpp)

target_link_libraries(procdraw
        procdraw_lib
        d3d11
        D3DCompiler)

# Tests

add_executable(procdraw_tests
        tests/ColorTests.cpp
        tests/InterpreterPrintTests.cpp
        tests/InterpreterReadTests.cpp
        tests/InterpreterTests.cpp
        tests/PrintTo.cpp
        tests/ProcdrawMathTests.cpp
        tests/TestUtils.cpp
        tests/VirtualMachineTests.cpp)

target_link_libraries(procdraw_tests
        procdraw_lib
        gtest
        gtest_main)

add_custom_target(test
        COMMAND procdraw_tests
        DEPENDS procdraw_tests)
