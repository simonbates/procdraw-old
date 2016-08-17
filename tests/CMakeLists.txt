get_filename_component(FUNCTION_SPEC_CHECKER_TESTDATA_DIR "function_spec_tests" ABSOLUTE)
get_filename_component(FUNCTION_SPECS_DIR "${PROJECT_SOURCE_DIR}/Documentation/functions" ABSOLUTE)
configure_file(procdraw_tests_config.h.in procdraw_tests_config.h)

add_executable(all_tests
    function_spec_tests/function_spec_checker.cpp
    function_spec_tests/function_spec_checker_test.cpp
    function_spec_tests/function_specs_test.cpp
    interpreter_tests/prettyprinter_test.cpp
    utils_tests/color_test.cpp
    utils_tests/font_utils_test.cpp
    utils_tests/line_buffer_test.cpp
    utils_tests/lsystem_test.cpp
    utils_tests/text_layout_test.cpp
    utils_tests/utils_test.cpp)

target_include_directories(all_tests
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(all_tests
    interpreter
    gtest
    gtest_main
    tinyxml2
    ${CMAKE_THREAD_LIBS_INIT})

add_test(all_tests all_tests)
