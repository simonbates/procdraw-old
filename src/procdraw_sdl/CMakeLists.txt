file(GLOB PROCDRAW_SDL_SOURCES "*.cpp" "*.h")

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src/procdraw_lib
                    ${SDL2_INCLUDE_DIRS}
                    ${OPENGL_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS})

add_executable(procdraw ${PROCDRAW_SDL_SOURCES})
target_link_libraries(procdraw
                      procdraw_lib
                      ${SDL2_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      readline)
