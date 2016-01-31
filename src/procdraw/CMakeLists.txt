include_directories(${PROJECT_SOURCE_DIR}/src/utils
                    ${PROJECT_SOURCE_DIR}/src/interpreter
                    ${PROJECT_SOURCE_DIR}/src/graphics
                    ${PROJECT_SOURCE_DIR}/src/alsa_midi
                    ${SDL2_INCLUDE_DIRS}
                    ${FT2_INCLUDE_DIRS}
                    ${OPENGL_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS})

add_executable(procdraw
               cli.cpp
               frame_counter.cpp
               main.cpp
               procdraw_app.cpp
               procdraw_app_lisp.cpp)

target_link_libraries(procdraw
                      interpreter
                      graphics
                      utils
                      alsa_midi
                      ${SDL2_LIBRARIES}
                      ${FT2_LIBRARIES}
                      ${ALSA_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      readline)
