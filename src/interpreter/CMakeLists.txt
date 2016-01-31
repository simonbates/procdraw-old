include_directories(${PROJECT_SOURCE_DIR}/src/utils)

add_library(interpreter
            lisp_functions.cpp
            lisp_interpreter.cpp
            lisp_memory.cpp
            lisp_printer.cpp
            lisp_reader.cpp
            signals.cpp)
