add_library(interpreter
    lisp_functions.cpp
    lisp_interpreter.cpp
    lisp_memory.cpp
    lisp_printer.cpp
    lisp_reader.cpp
    signals.cpp)

target_include_directories(interpreter
    PUBLIC .)

target_link_libraries(interpreter
    utils)
