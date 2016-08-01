add_library(graphics
    camera.cpp
    ft_text_renderer.cpp
    glrenderer.cpp
    gl_util.cpp
    sdl_util.cpp)

target_include_directories(graphics
    PUBLIC ${PROCDRAW_INCLUDE_DIR}
    PUBLIC ${FT2_INCLUDE_DIRS}
    PRIVATE ${SDL2_INCLUDE_DIRS}
    PRIVATE ${OPENGL_INCLUDE_DIRS}
    PRIVATE ${GLEW_INCLUDE_DIRS})

target_link_libraries(graphics
    utils
    ${SDL2_LIBRARIES}
    ${FT2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES})
