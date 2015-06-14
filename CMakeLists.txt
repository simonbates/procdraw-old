cmake_minimum_required(VERSION 2.8.12)

project(Procdraw)

enable_testing()

# Find dependencies

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS}
                    ${OPENGL_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS})

# Platform-specific compiler settings

if (WIN32)
    add_definitions(-DUNICODE -D_UNICODE -D_USE_MATH_DEFINES)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# libprocdraw

add_library(libprocdraw
            color.cc
            glrenderer.cc
            line_buffer.cc
            lisp_functions.cc
            lisp_interpreter.cc
            lisp_memory.cc
            lisp_reader.cc
            lsystem.cc
            procdraw_app_sdl.cc
            procdraw_app_sdl_lisp.cc
            repl.cc
            sdl_util.cc
            signals.cc
            util.cc)

set_target_properties(libprocdraw PROPERTIES OUTPUT_NAME procdraw)

# procdraw

add_executable(procdraw main.cc)

target_link_libraries(procdraw
                      libprocdraw
                      ${SDL2_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      readline)

# Tests

add_subdirectory(tests)
