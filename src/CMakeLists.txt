set(PROCDRAW_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# procdraw_lib

add_library(procdraw_lib
    procdraw/graphics/bitmap_text_renderer.cpp
    procdraw/graphics/bitmap_text_renderer.h
    procdraw/graphics/camera.cpp
    procdraw/graphics/camera.h
    procdraw/graphics/console.cpp
    procdraw/graphics/console.h
    procdraw/graphics/console_line.h
    procdraw/graphics/font_config.h
    procdraw/graphics/frame_counter.cpp
    procdraw/graphics/frame_counter.h
    procdraw/graphics/freetype_font_loader.cpp
    procdraw/graphics/freetype_font_loader.h
    procdraw/graphics/glrenderer.cpp
    procdraw/graphics/glrenderer.h
    procdraw/graphics/gl_util.cpp
    procdraw/graphics/gl_util.h
    procdraw/graphics/sdl_util.cpp
    procdraw/graphics/sdl_util.h
    procdraw/graphics/vertex.h
    procdraw/interpreter/lisp_functions.cpp
    procdraw/interpreter/lisp_functions.h
    procdraw/interpreter/lisp_interpreter.cpp
    procdraw/interpreter/lisp_interpreter.h
    procdraw/interpreter/lisp_memory.cpp
    procdraw/interpreter/lisp_memory.h
    procdraw/interpreter/lisp_reader.cpp
    procdraw/interpreter/lisp_reader.h
    procdraw/interpreter/prettyprinter.cpp
    procdraw/interpreter/prettyprinter.h
    procdraw/interpreter/signals.cpp
    procdraw/interpreter/signals.h
    procdraw/bitmap_font_metrics.cpp
    procdraw/bitmap_font_metrics.h
    procdraw/bitmap_glyph_metrics.h
    procdraw/color.cpp
    procdraw/color.h
    procdraw/command_processor.h
    procdraw/line_buffer.cpp
    procdraw/line_buffer.h
    procdraw/lsystem.cpp
    procdraw/lsystem.h
    procdraw/math.cpp
    procdraw/math.h
    procdraw/midi_listener.h
    procdraw/procdraw_app.cpp
    procdraw/procdraw_app.h
    procdraw/procdraw_app_lisp.cpp
    procdraw/procdraw_app_lisp.h
    procdraw/simple_moving_average.h
    procdraw/text_layout_engine.h
    procdraw/text_layout.h)

target_include_directories(procdraw_lib
    PUBLIC ${PROCDRAW_SRC_DIR}
    PUBLIC ${GLM_DIR}
    PUBLIC ${SDL2_INCLUDE_DIRS}
    PRIVATE ${OPENGL_INCLUDE_DIRS})

target_link_libraries(procdraw_lib
    freetype2
    gl3w
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARIES})

# procdraw_alsa_midi

add_library(procdraw_alsa_midi
    procdraw/alsa_midi/alsa_midi_client.cpp
    procdraw/alsa_midi/alsa_midi_client.h)

target_include_directories(procdraw_alsa_midi
    PUBLIC ${PROCDRAW_SRC_DIR}
    PRIVATE ${ALSA_INCLUDE_DIRS})

target_link_libraries(procdraw_alsa_midi
    ${ALSA_LIBRARIES})

# procdraw_test_support

add_library(procdraw_test_support
    procdraw/test_support/function_spec_checker.cpp
    procdraw/test_support/function_spec_checker.h)

target_include_directories(procdraw_test_support
    PUBLIC ${PROCDRAW_SRC_DIR})

target_link_libraries(procdraw_test_support
    procdraw_lib
    tinyxml2)

# procdraw_main

add_executable(procdraw_main
    procdraw/main/main.cpp)

target_include_directories(procdraw_main
    PRIVATE ${PROCDRAW_SRC_DIR})

target_link_libraries(procdraw_main
    procdraw_lib)

target_link_libraries(procdraw_main procdraw_alsa_midi)

set_target_properties(procdraw_main
    PROPERTIES OUTPUT_NAME procdraw)

if(WIN32)
    set_target_properties(procdraw_main
        PROPERTIES WIN32_EXECUTABLE TRUE)
    add_custom_command(TARGET procdraw_main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PROCDRAW_SDL2_DLL}
            $<TARGET_FILE_DIR:procdraw_main>)
endif()
