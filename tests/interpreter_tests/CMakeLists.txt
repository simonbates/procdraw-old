include_directories(${CATCH_INCLUDE_DIR})

add_executable(interpreter_tests
    main.cpp
    test_lisp_interpreter.cpp
    test_lisp_reader.cpp
    test_signals.cpp)

target_link_libraries(interpreter_tests
    interpreter)

add_test(interpreter_tests interpreter_tests)
