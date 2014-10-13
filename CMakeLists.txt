cmake_minimum_required(VERSION 2.8)

project(ProcDraw)

enable_testing()

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(src/procdraw_lib)
add_subdirectory(src/procdraw_win)
add_subdirectory(src/procdraw_winmain)
add_subdirectory(tests/procdraw_lib_tests)
