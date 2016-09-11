add_library(graphics
    bitmap_text_renderer.cpp
    camera.cpp
    freetype_font_loader.cpp
    glrenderer.cpp
    gl_util.cpp
    sdl_util.cpp)

target_include_directories(graphics
    PUBLIC ${PROCDRAW_INCLUDE_DIR}
    PRIVATE ${SDL2_INCLUDE_DIRS}
    PRIVATE ${OPENGL_INCLUDE_DIRS})

target_link_libraries(graphics
    utils
    gl3w
    freetype2
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES})
