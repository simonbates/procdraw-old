cmake_minimum_required(VERSION 2.8.12)

project(Procdraw)

# Generate compile_commands.json for use with irony-mode
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

# Find dependencies

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    include(FindPkgConfig)
    pkg_search_module(SDL2 REQUIRED sdl2)
    pkg_search_module(FT2 REQUIRED freetype2)
    pkg_search_module(ALSA REQUIRED alsa)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(Threads REQUIRED)
endif()

# Catch

set(CATCH_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party/Catch/single_include)

# GLM

set(GLM_DIR ${PROJECT_SOURCE_DIR}/third_party/glm)

# Google Test

set(GTEST_DIR ${PROJECT_SOURCE_DIR}/third_party/google/googletest)

add_library(gtest ${GTEST_DIR}/src/gtest-all.cc)
target_include_directories(gtest
    PUBLIC ${GTEST_DIR}/include
    PRIVATE ${GTEST_DIR})

add_library(gtest_main ${GTEST_DIR}/src/gtest_main.cc)
target_include_directories(gtest_main PUBLIC ${GTEST_DIR}/include)

# TinyXML-2

set(TINYXML2_DIR ${PROJECT_SOURCE_DIR}/third_party/tinyxml2)
add_library(tinyxml2 ${TINYXML2_DIR}/tinyxml2.cpp)
target_include_directories(tinyxml2 PUBLIC ${TINYXML2_DIR})

# Platform-specific compiler settings

if (WIN32)
    add_definitions(-DUNICODE -D_UNICODE -D_USE_MATH_DEFINES)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wpedantic -Wall")
endif()

# Projects

add_subdirectory(src/utils)
add_subdirectory(src/interpreter)
add_subdirectory(tests/utils_tests)
add_subdirectory(tests/interpreter_tests)
add_subdirectory(tests/function_spec_tests)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_subdirectory(src/graphics)
    add_subdirectory(src/alsa_midi)
    add_subdirectory(src/procdraw)
endif()
