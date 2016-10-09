cmake_minimum_required(VERSION 2.8.12)

project(Procdraw)

# CTest

enable_testing()

# Find dependencies

find_package(OpenGL REQUIRED)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    include(FindPkgConfig)
    pkg_search_module(SDL2 REQUIRED sdl2)
    find_package(Threads REQUIRED)
    set(PROCDRAW_ENABLE_ALSA_MIDI ON)
elseif(WIN32)
    set(PROCDRAW_SDL2_DIR $ENV{PROCDRAW_SDL2_DIR})
    set(PROCDRAW_SDL2_LIB "${PROCDRAW_SDL2_DIR}/lib/x86/SDL2.lib")
    set(PROCDRAW_SDL2_DLL "${PROCDRAW_SDL2_DIR}/lib/x86/SDL2.dll")
    set(SDL2_INCLUDE_DIRS "${PROCDRAW_SDL2_DIR}/include")
    set(SDL2_LIBRARIES ${PROCDRAW_SDL2_LIB})
endif()

if(PROCDRAW_ENABLE_ALSA_MIDI)
    find_package(ALSA REQUIRED)
    add_definitions(-DPROCDRAW_ENABLE_ALSA_MIDI)
endif()

# Configure SDL2 include names
#
# On Linux, the SDL2 headers live inside a directory "SDL2". But on
# Windows, the build of SDL2 does not include a parent "SDL2"
# directory for the headers.

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_definitions(-DPROCDRAW_SDL2_H=<SDL2/SDL.h>)
    add_definitions(-DPROCDRAW_SDL2_OPENGL_H=<SDL2/SDL_opengl.h>)
elseif(WIN32)
    add_definitions(-DPROCDRAW_SDL2_H=<SDL.h>)
    add_definitions(-DPROCDRAW_SDL2_OPENGL_H=<SDL_opengl.h>)
endif()

# Catch

set(CATCH_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party/Catch/single_include)

# FreeType 2

set(FREETYPE2_DIR ${PROJECT_SOURCE_DIR}/third_party/freetype-2.6.5)
add_library(freetype2
    ${FREETYPE2_DIR}/src/base/ftsystem.c
    ${FREETYPE2_DIR}/src/base/ftinit.c
    ${FREETYPE2_DIR}/src/base/ftdebug.c
    ${FREETYPE2_DIR}/src/base/ftbase.c
    ${FREETYPE2_DIR}/src/base/ftbitmap.c
    ${FREETYPE2_DIR}/src/sfnt/sfnt.c
    ${FREETYPE2_DIR}/src/truetype/truetype.c
    ${FREETYPE2_DIR}/src/raster/raster.c
    ${FREETYPE2_DIR}/src/smooth/smooth.c
    ${FREETYPE2_DIR}/src/autofit/autofit.c
    ${FREETYPE2_DIR}/src/pshinter/pshinter.c
    ${FREETYPE2_DIR}/src/psnames/psnames.c)
target_include_directories(freetype2
    PUBLIC ${PROJECT_SOURCE_DIR}/third_party/freetype2-custom
    PUBLIC ${FREETYPE2_DIR}/include)
target_compile_definitions(freetype2 PRIVATE FT2_BUILD_LIBRARY)

# GL3W

set(GL3W_DIR ${PROJECT_SOURCE_DIR}/third_party/gl3w_out)
add_library(gl3w ${GL3W_DIR}/src/gl3w.c)
target_include_directories(gl3w PUBLIC ${GL3W_DIR}/include)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(gl3w dl)
endif()

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

# Procdraw include directory

set(PROCDRAW_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# Platform-specific compiler settings

if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE -D_USE_MATH_DEFINES)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wpedantic -Wall")
endif()

# Projects

add_subdirectory(src/utils)
add_subdirectory(src/interpreter)
add_subdirectory(src/graphics)

if(PROCDRAW_ENABLE_ALSA_MIDI)
    add_subdirectory(src/alsa_midi)
endif()

add_subdirectory(src/procdraw)
add_subdirectory(src/test)
add_subdirectory(tests)
add_subdirectory(tests/interpreter_tests)
