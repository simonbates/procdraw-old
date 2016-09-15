add_library(procdraw_test
    function_spec_checker.cpp)

target_include_directories(procdraw_test
    PUBLIC ${PROCDRAW_INCLUDE_DIR})

target_link_libraries(procdraw_test
    tinyxml2)
