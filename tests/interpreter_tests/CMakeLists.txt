file(GLOB INTERPRETER_TESTS_SOURCES "*.cpp")

include_directories(${CATCH_INCLUDE_DIR})

add_executable(interpreter_tests
    ${INTERPRETER_TESTS_SOURCES})

target_link_libraries(interpreter_tests
    interpreter)

add_test(interpreter_tests interpreter_tests)
