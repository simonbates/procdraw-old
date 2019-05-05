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
        ProcdrawLib/ProcdrawMath.cpp)

target_include_directories(procdraw_lib PUBLIC ProcdrawLib)

# Tests

add_executable(procdraw_tests
        tests/ProcdrawMathTests.cpp)

target_link_libraries(procdraw_tests
        procdraw_lib
        gtest
        gtest_main)

add_custom_target(test
        COMMAND procdraw_tests
        DEPENDS procdraw_tests)
