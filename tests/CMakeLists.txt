get_filename_component(FUNCTION_SPEC_CHECKER_TESTDATA_DIR "testdata" ABSOLUTE)
get_filename_component(FUNCTION_SPECS_DIR "${PROJECT_SOURCE_DIR}/Documentation/functions" ABSOLUTE)

add_definitions(-DPROCDRAW_FUNCTION_SPEC_CHECKER_TESTDATA_DIR="${FUNCTION_SPEC_CHECKER_TESTDATA_DIR}")
add_definitions(-DPROCDRAW_FUNCTION_SPECS_DIR="${FUNCTION_SPECS_DIR}")

add_executable(procdraw_all_tests
    interpreter/lisp_interpreter_test.cpp
    interpreter/lisp_reader_test.cpp
    interpreter/prettyprinter_test.cpp
    interpreter/signals_test.cpp
    color_test.cpp
    function_spec_checker_test.cpp
    function_specs_test.cpp
    line_buffer_test.cpp
    lsystem_test.cpp
    math_test.cpp
    simple_moving_average_test.cpp
    text_layout_engine_test.cpp
    text_layout_test.cpp)

target_link_libraries(procdraw_all_tests
    procdraw_lib
    procdraw_test_support
    gtest
    gtest_main
    ${CMAKE_THREAD_LIBS_INIT})

add_custom_target(check
    COMMAND procdraw_all_tests
    DEPENDS procdraw_all_tests)
