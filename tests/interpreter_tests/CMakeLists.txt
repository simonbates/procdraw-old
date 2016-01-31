file(GLOB INTERPRETER_TESTS_SOURCES "*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/src/interpreter
                    ${PROJECT_SOURCE_DIR}/src/utils
                    ${PROJECT_SOURCE_DIR}/third_party/catch)

add_executable(interpreter_tests ${INTERPRETER_TESTS_SOURCES})
target_link_libraries(interpreter_tests
                      interpreter
                      utils)

add_test(interpreter_tests interpreter_tests)
