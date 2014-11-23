file(GLOB PROCDRAW_LIB_TESTS_SOURCES "*.cpp" "*.h")

include_directories(${PROJECT_SOURCE_DIR}/src/procdraw_lib)
include_directories(${PROJECT_SOURCE_DIR}/third_party/catch)

add_executable(procdraw_lib_tests ${PROCDRAW_LIB_TESTS_SOURCES})
target_link_libraries(procdraw_lib_tests procdraw_lib)

add_custom_target(run_procdraw_lib_tests COMMAND procdraw_lib_tests)
add_dependencies(test run_procdraw_lib_tests)
