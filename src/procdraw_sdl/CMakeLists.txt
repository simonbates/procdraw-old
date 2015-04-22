file(GLOB PROCDRAW_SDL_SOURCES "*.cpp" "*.h")

add_executable(procdraw ${PROCDRAW_SDL_SOURCES})

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
find_package(OpenGL REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
target_link_libraries(procdraw ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
