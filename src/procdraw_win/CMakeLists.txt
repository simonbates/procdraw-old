file(GLOB PROCDRAW_WIN_SOURCES "*.cpp" "*.h")

include_directories(${PROJECT_SOURCE_DIR}/src/procdraw_lib)

add_library(procdraw_win ${PROCDRAW_WIN_SOURCES})
