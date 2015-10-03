pkg_search_module(LIBXML2 REQUIRED libxml-2.0)

include_directories(${PROJECT_SOURCE_DIR}
                    ${LIBXML2_INCLUDE_DIRS})

add_executable(test_function_spec test_function_spec.cc)

target_link_libraries(test_function_spec
                      libprocdraw
                      ${LIBXML2_LIBRARIES})

# Tests for test_function_spec

function(add_test_function_spec_failure test_name spec_filename num_tests expected_output)
    add_test("test_function_spec_${test_name}_fails" test_function_spec ${spec_filename} ${num_tests})
    set_tests_properties("test_function_spec_${test_name}_fails" PROPERTIES WILL_FAIL true)

    add_test("test_function_spec_${test_name}_msg" test_function_spec ${spec_filename} ${num_tests})
    set_tests_properties("test_function_spec_${test_name}_msg"
                         PROPERTIES
                         PASS_REGULAR_EXPRESSION "${expected_output}")
endfunction()

add_test_function_spec_failure(non_existing_file
                               NON_EXISTING_FILE 0
                               "File not found: NON_EXISTING_FILE")

add_test_function_spec_failure(testcase1
                               ${PROJECT_SOURCE_DIR}/tests/function_spec_tests/testcase1.xml 1
                               "EXPR: \\(\\+ 1\\) EXPECTED: 0 ACTUAL: 1")

add_test_function_spec_failure(testcase2
                               ${PROJECT_SOURCE_DIR}/tests/function_spec_tests/testcase2.xml 2
                               "EXPR: \\(\\+ 2\\) EXPECTED: 0 ACTUAL: 2")

add_test_function_spec_failure(testcase3
                               ${PROJECT_SOURCE_DIR}/tests/function_spec_tests/testcase3.xml 2
                               "EXPR: \\(\\+ 3\\) EXPECTED: 0 ACTUAL: 3")

add_test_function_spec_failure(too_few
                               ${PROJECT_SOURCE_DIR}/tests/function_spec_tests/passing.xml 3
                               "EXPECTED 3 TESTS BUT 2 WERE RUN")

add_test_function_spec_failure(too_many
                               ${PROJECT_SOURCE_DIR}/tests/function_spec_tests/passing.xml 1
                               "EXPECTED 1 TESTS BUT 2 WERE RUN")

# Function-spec tests

add_test(test_sum test_function_spec ${PROJECT_SOURCE_DIR}/Documentation/sum.xml 5)
