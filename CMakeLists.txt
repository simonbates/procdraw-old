cmake_minimum_required(VERSION 2.8)

project(ProcDraw)

add_custom_target(test)

# Platform-specific compiler settings

if (WIN32)
    add_definitions(-DUNICODE -D_UNICODE -D_USE_MATH_DEFINES)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# Projects

add_subdirectory(src/procdraw_lib)
add_subdirectory(tests/procdraw_lib_tests)

if (WIN32)
    add_subdirectory(src/procdraw_win)
    add_subdirectory(src/procdraw_winmain)
endif()
