cmake_minimum_required(VERSION 2.8.12)

project(Procdraw)

# Generate compile_commands.json for use with irony-mode
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

# Find dependencies

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(FT2 REQUIRED freetype2)
pkg_search_module(ALSA REQUIRED alsa)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

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
add_subdirectory(src/graphics)
add_subdirectory(src/alsa_midi)
add_subdirectory(src/procdraw)
add_subdirectory(tests/utils_tests)
add_subdirectory(tests/interpreter_tests)
add_subdirectory(tests/function_spec_tests)

# Documentation

add_subdirectory(Documentation)
