include_directories(${PROJECT_SOURCE_DIR}/src/utils
                    ${SDL2_INCLUDE_DIRS}
                    ${FT2_INCLUDE_DIRS}
                    ${OPENGL_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS})

add_library(graphics
            camera.cpp
            ft_text_renderer.cpp
            glrenderer.cpp
            gl_util.cpp
            sdl_util.cpp)
