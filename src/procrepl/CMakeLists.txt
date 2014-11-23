file(GLOB PROCREPL_SOURCES "*.cpp" "*.h")

include_directories(${PROJECT_SOURCE_DIR}/src/procdraw_lib)

add_executable(procrepl ${PROCREPL_SOURCES})
target_link_libraries(procrepl procdraw_lib)
