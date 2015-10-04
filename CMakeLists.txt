cmake_minimum_required(VERSION 2.8.12)

project(Procdraw)

# Generate compile_commands.json for use with irony-mode
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

# Find dependencies

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(FT2 REQUIRED freetype2)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS}
                    ${FT2_INCLUDE_DIRS}
                    ${OPENGL_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS})

# Platform-specific compiler settings

if (WIN32)
    add_definitions(-DUNICODE -D_UNICODE -D_USE_MATH_DEFINES)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wpedantic -Wall")
endif()

# libprocdraw

add_library(libprocdraw
            camera.cc
            cli.cc
            color.cc
            frame_counter.cc
            ft_text_renderer.cc
            glrenderer.cc
            gl_util.cc
            line_buffer.cc
            lisp_functions.cc
            lisp_interpreter.cc
            lisp_memory.cc
            lisp_printer.cc
            lisp_reader.cc
            lsystem.cc
            procdraw_app.cc
            procdraw_app_lisp.cc
            sdl_util.cc
            signals.cc
            util.cc)

set_target_properties(libprocdraw PROPERTIES OUTPUT_NAME procdraw)

# procdraw

add_executable(procdraw main.cc)

target_link_libraries(procdraw
                      libprocdraw
                      ${SDL2_LIBRARIES}
                      ${FT2_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      readline)

# Tests

add_subdirectory(tests/unit_tests)
add_subdirectory(tests/function_spec_tests)

# Documentation

add_subdirectory(Documentation)
