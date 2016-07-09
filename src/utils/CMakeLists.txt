add_library(utils
    color.cpp
    font_utils.cpp
    line_buffer.cpp
    lsystem.cpp
    utils.cpp)

target_include_directories(utils
    PUBLIC .
    PUBLIC ${GLM_DIR})
