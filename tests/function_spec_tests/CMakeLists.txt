add_executable(test_function_spec
    test_function_spec.cpp)

target_link_libraries(test_function_spec
    interpreter
    tinyxml2)

function(add_spec_failure name spec numtests expected)
    add_test("function_spec_${name}_fails" test_function_spec ${spec} ${numtests})
    set_tests_properties("function_spec_${name}_fails" PROPERTIES WILL_FAIL true)

    add_test("function_spec_${name}_msg" test_function_spec ${spec} ${numtests})
    set_tests_properties("function_spec_${name}_msg"
                         PROPERTIES
                         PASS_REGULAR_EXPRESSION "${expected}")
endfunction()

function(add_function_test name numtests)
    add_test(${name} test_function_spec "${PROJECT_SOURCE_DIR}/Documentation/functions/${name}.xml" ${numtests})
endfunction()

# Tests for test_function_spec

add_spec_failure(non_existing_file NON_EXISTING_FILE 0
                 "Error loading file: NON_EXISTING_FILE")

add_spec_failure(testcase1
                 ${PROJECT_SOURCE_DIR}/tests/function_spec_tests/testcase1.xml 1
                 "EXPR: \\(\\+ 1\\) EXPECTED: 0 ACTUAL: 1")

add_spec_failure(testcase2
                 ${PROJECT_SOURCE_DIR}/tests/function_spec_tests/testcase2.xml 2
                 "EXPR: \\(\\+ 2\\) EXPECTED: 0 ACTUAL: 2")

add_spec_failure(testcase3
                 ${PROJECT_SOURCE_DIR}/tests/function_spec_tests/testcase3.xml 2
                 "EXPR: \\(\\+ 3\\) EXPECTED: 0 ACTUAL: 3")

add_spec_failure(too_few
                 ${PROJECT_SOURCE_DIR}/tests/function_spec_tests/passing.xml 3
                 "EXPECTED 3 TESTS BUT 2 WERE RUN")

add_spec_failure(too_many
                 ${PROJECT_SOURCE_DIR}/tests/function_spec_tests/passing.xml 1
                 "EXPECTED 1 TESTS BUT 2 WERE RUN")

# Function tests

add_function_test(clear 5)
add_function_test(cons 3)
add_function_test(difference 5)
add_function_test(eq 7)
add_function_test(functionp 4)
add_function_test(get 5)
add_function_test(keys 4)
add_function_test(make-dict 5)
add_function_test(memb 6)
add_function_test(product 5)
add_function_test(put 5)
add_function_test(quotient 4)
add_function_test(sum 5)
