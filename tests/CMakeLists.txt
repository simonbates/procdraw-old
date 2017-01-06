# procdraw_core_tests

get_filename_component(FUNCTION_SPEC_CHECKER_TESTDATA_DIR "core_tests/testdata" ABSOLUTE)
get_filename_component(FUNCTION_SPECS_DIR "${PROJECT_SOURCE_DIR}/Documentation/functions" ABSOLUTE)

add_definitions(-DPROCDRAW_FUNCTION_SPEC_CHECKER_TESTDATA_DIR="${FUNCTION_SPEC_CHECKER_TESTDATA_DIR}")
add_definitions(-DPROCDRAW_FUNCTION_SPECS_DIR="${FUNCTION_SPECS_DIR}")

add_executable(procdraw_core_tests
    core_tests/interpreter/lisp_interpreter_test.cpp
    core_tests/interpreter/lisp_reader_test.cpp
    core_tests/interpreter/prettyprinter_test.cpp
    core_tests/interpreter/signals_test.cpp
    core_tests/color_test.cpp
    core_tests/function_spec_checker_test.cpp
    core_tests/function_specs_test.cpp
    core_tests/line_buffer_test.cpp
    core_tests/lsystem_test.cpp
    core_tests/math_test.cpp
    core_tests/simple_moving_average_test.cpp
    core_tests/text_layout_engine_test.cpp
    core_tests/text_layout_test.cpp)

target_link_libraries(procdraw_core_tests
    procdraw_core
    procdraw_test_support
    gtest
    gtest_main
    ${CMAKE_THREAD_LIBS_INIT})

add_custom_target(check
    COMMAND procdraw_core_tests
    DEPENDS procdraw_core_tests)
