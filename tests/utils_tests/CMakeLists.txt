include_directories(${CATCH_INCLUDE_DIR})

add_executable(utils_tests
    main.cpp
    test_font_utils.cpp)

target_link_libraries(utils_tests
    utils)

add_test(utils_tests utils_tests)
