file(GLOB PROCDRAW_LIB_SOURCES "*.cpp" "*.h")

add_library(procdraw_lib ${PROCDRAW_LIB_SOURCES})

set_target_properties(procdraw_lib PROPERTIES OUTPUT_NAME procdraw)
