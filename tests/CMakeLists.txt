get_filename_component(FUNCTION_SPEC_CHECKER_TESTDATA_DIR "function_spec_tests" ABSOLUTE)
get_filename_component(FUNCTION_SPECS_DIR "${PROJECT_SOURCE_DIR}/Documentation/functions" ABSOLUTE)

add_definitions(-DPROCDRAW_FUNCTION_SPEC_CHECKER_TESTDATA_DIR="${FUNCTION_SPEC_CHECKER_TESTDATA_DIR}")
add_definitions(-DPROCDRAW_FUNCTION_SPECS_DIR="${FUNCTION_SPECS_DIR}")

add_executable(all_tests
    function_spec_tests/function_spec_checker_test.cpp
    function_spec_tests/function_specs_test.cpp
    interpreter_tests/prettyprinter_test.cpp
    utils_tests/color_test.cpp
    utils_tests/line_buffer_test.cpp
    utils_tests/lsystem_test.cpp
    utils_tests/text_layout_engine_test.cpp
    utils_tests/text_layout_test.cpp
    utils_tests/utils_test.cpp)

target_link_libraries(all_tests
    procdraw_lib
    procdraw_test
    gtest
    gtest_main
    ${CMAKE_THREAD_LIBS_INIT})

add_test(all_tests all_tests)
