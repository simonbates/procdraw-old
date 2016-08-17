add_library(interpreter
    lisp_functions.cpp
    lisp_interpreter.cpp
    lisp_memory.cpp
    lisp_printer.cpp
    lisp_reader.cpp
    prettyprinter.cpp
    signals.cpp)

target_include_directories(interpreter
    PUBLIC ${PROCDRAW_INCLUDE_DIR})

target_link_libraries(interpreter
    utils)
