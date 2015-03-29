file(GLOB PROCDRAW_WINMAIN_SOURCES "*.cpp" "*.h" "*.rc")

include_directories(${PROJECT_SOURCE_DIR}/src/procdraw_lib)
include_directories(${PROJECT_SOURCE_DIR}/src/procdraw_win)

add_executable(procdraw_winmain WIN32 ${PROCDRAW_WINMAIN_SOURCES})

target_link_libraries(procdraw_winmain procdraw_lib procdraw_win d3d11 D3DCompiler)

set_target_properties(procdraw_winmain PROPERTIES OUTPUT_NAME procdraw)
