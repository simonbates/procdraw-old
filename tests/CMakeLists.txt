file(GLOB UNIT_TESTS_SOURCES "*.cc")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/third_party/catch)

add_executable(unit_tests ${UNIT_TESTS_SOURCES})
target_link_libraries(unit_tests libprocdraw)

add_test(unit_tests unit_tests)
