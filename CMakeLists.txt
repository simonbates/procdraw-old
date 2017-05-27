cmake_minimum_required(VERSION 2.8.12)

project(Procdraw)

enable_testing()

# Find dependencies

find_package(ALSA REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(FT2 REQUIRED freetype2)

add_definitions(-DPROCDRAW_SDL2_H=<SDL2/SDL.h>)
add_definitions(-DPROCDRAW_SDL2_OPENGL_H=<SDL2/SDL_opengl.h>)

# GL3W

set(GL3W_DIR ${PROJECT_SOURCE_DIR}/third_party/gl3w_out)
add_library(gl3w ${GL3W_DIR}/src/gl3w.c)
target_include_directories(gl3w PUBLIC ${GL3W_DIR}/include)
target_link_libraries(gl3w dl)

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

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wpedantic -Wall")
endif()

set(PROCDRAW_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# procdraw_lib

add_library(procdraw_lib
        src/procdraw/alsa_midi/alsa_midi_client.cpp
        src/procdraw/graphics/bitmap_text_renderer.cpp
        src/procdraw/graphics/camera.cpp
        src/procdraw/graphics/console.cpp
        src/procdraw/graphics/frame_counter.cpp
        src/procdraw/graphics/freetype_font_loader.cpp
        src/procdraw/graphics/glrenderer.cpp
        src/procdraw/graphics/gl_util.cpp
        src/procdraw/graphics/sdl_util.cpp
        src/procdraw/interpreter/lisp_functions.cpp
        src/procdraw/interpreter/lisp_interpreter.cpp
        src/procdraw/interpreter/lisp_memory.cpp
        src/procdraw/interpreter/lisp_reader.cpp
        src/procdraw/interpreter/prettyprinter.cpp
        src/procdraw/interpreter/signals.cpp
        src/procdraw/bitmap_font_metrics.cpp
        src/procdraw/color.cpp
        src/procdraw/line_buffer.cpp
        src/procdraw/lsystem.cpp
        src/procdraw/math.cpp
        src/procdraw/procdraw_app.cpp
        src/procdraw/procdraw_app_lisp.cpp)

target_include_directories(procdraw_lib
        PUBLIC ${PROCDRAW_SRC_DIR}
        PUBLIC ${GLM_DIR}
        PUBLIC ${FT2_INCLUDE_DIRS}
        PUBLIC ${SDL2_INCLUDE_DIRS}
        PRIVATE ${ALSA_INCLUDE_DIRS}
        PRIVATE ${OPENGL_INCLUDE_DIRS})

target_link_libraries(procdraw_lib
        gl3w
        ${ALSA_LIBRARIES}
        ${FT2_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARIES})

# procdraw_main

add_executable(procdraw_main
        src/procdraw/main/main.cpp)

target_include_directories(procdraw_main
        PRIVATE ${PROCDRAW_SRC_DIR})

target_link_libraries(procdraw_main
        procdraw_lib)

set_target_properties(procdraw_main
        PROPERTIES OUTPUT_NAME procdraw)

# Tests

get_filename_component(FUNCTION_SPEC_CHECKER_TESTDATA_DIR "${PROJECT_SOURCE_DIR}/tests/testdata" ABSOLUTE)
get_filename_component(FUNCTION_SPECS_DIR "${PROJECT_SOURCE_DIR}/Documentation/docs/functions" ABSOLUTE)

add_definitions(-DPROCDRAW_FUNCTION_SPEC_CHECKER_TESTDATA_DIR="${FUNCTION_SPEC_CHECKER_TESTDATA_DIR}")
add_definitions(-DPROCDRAW_FUNCTION_SPECS_DIR="${FUNCTION_SPECS_DIR}")

add_executable(procdraw_all_tests
        src/procdraw/test_support/function_spec_checker.cpp
        tests/interpreter_tests/lisp_interpreter_test.cpp
        tests/interpreter_tests/lisp_reader_test.cpp
        tests/interpreter_tests/prettyprinter_test.cpp
        tests/interpreter_tests/signals_test.cpp
        tests/color_test.cpp
        tests/function_spec_checker_test.cpp
        tests/function_specs_test.cpp
        tests/line_buffer_test.cpp
        tests/lsystem_test.cpp
        tests/math_test.cpp
        tests/simple_moving_average_test.cpp
        tests/text_layout_engine_test.cpp
        tests/text_layout_test.cpp)

target_link_libraries(procdraw_all_tests
        procdraw_lib
        gtest
        gtest_main
        tinyxml2
        ${CMAKE_THREAD_LIBS_INIT})

add_test(procdraw_all_tests procdraw_all_tests)
