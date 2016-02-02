file(GLOB UTILS_TESTS_SOURCES "*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/third_party/catch)

add_executable(utils_tests
    ${UTILS_TESTS_SOURCES})

target_link_libraries(utils_tests
    utils)

add_test(utils_tests utils_tests)
