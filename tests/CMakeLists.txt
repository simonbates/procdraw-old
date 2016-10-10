get_filename_component(FUNCTION_SPEC_CHECKER_TESTDATA_DIR "testdata" ABSOLUTE)
get_filename_component(FUNCTION_SPECS_DIR "${PROJECT_SOURCE_DIR}/Documentation/functions" ABSOLUTE)

add_definitions(-DPROCDRAW_FUNCTION_SPEC_CHECKER_TESTDATA_DIR="${FUNCTION_SPEC_CHECKER_TESTDATA_DIR}")
add_definitions(-DPROCDRAW_FUNCTION_SPECS_DIR="${FUNCTION_SPECS_DIR}")

add_executable(procdraw_tests
    interpreter/prettyprinter_test.cpp
    color_test.cpp
    function_spec_checker.cpp
    function_spec_checker_test.cpp
    function_specs_test.cpp
    line_buffer_test.cpp
    lsystem_test.cpp
    text_layout_engine_test.cpp
    text_layout_test.cpp
    utils_test.cpp)

target_link_libraries(procdraw_tests
    procdraw_lib
    gtest
    gtest_main
    tinyxml2
    ${CMAKE_THREAD_LIBS_INIT})

add_test(procdraw_tests procdraw_tests)

# Legacy Catch tests

add_executable(procdraw_catch_tests
    legacy_catch_tests/main.cpp
    legacy_catch_tests/test_lisp_interpreter.cpp
    legacy_catch_tests/test_lisp_reader.cpp
    legacy_catch_tests/test_signals.cpp)

target_include_directories(procdraw_catch_tests
    PRIVATE ${CATCH_INCLUDE_DIR})

target_link_libraries(procdraw_catch_tests
    procdraw_lib)

add_test(procdraw_catch_tests procdraw_catch_tests)
