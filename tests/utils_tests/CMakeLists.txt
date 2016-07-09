include_directories(${CATCH_INCLUDE_DIR})

add_executable(utils_tests
    main.cpp
    test_color.cpp
    test_font_utils.cpp
    test_line_buffer.cpp
    test_lsystem.cpp
    test_utils.cpp)

target_link_libraries(utils_tests
    utils)

add_test(utils_tests utils_tests)

# Google Test

add_executable(utils_tests_gtest
    color_test.cpp
    lsystem_test.cpp
    utils_test.cpp)

target_link_libraries(utils_tests_gtest
    utils
    gtest
    gtest_main
    ${CMAKE_THREAD_LIBS_INIT})

add_test(utils_tests_gtest utils_tests_gtest)
