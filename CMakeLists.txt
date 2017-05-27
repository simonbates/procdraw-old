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

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wpedantic -Wall")
endif()

# Add Procdraw source and tests

add_subdirectory(src)
add_subdirectory(tests)
