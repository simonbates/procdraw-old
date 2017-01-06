set(PROCDRAW_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# procdraw_lib

add_library(procdraw_lib
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
    procdraw/simple_moving_average.h
    procdraw/text_layout_engine.h
    procdraw/text_layout.h)

target_include_directories(procdraw_lib
    PUBLIC ${PROCDRAW_SRC_DIR}
    PUBLIC ${GLM_DIR})

target_link_libraries(procdraw_lib
    freetype2)

# procdraw_alsa_midi

if(ENABLE_ALSA_MIDI)
    add_library(procdraw_alsa_midi
        procdraw/alsa_midi/alsa_midi_client.cpp
        procdraw/alsa_midi/alsa_midi_client.h)

    target_include_directories(procdraw_alsa_midi
        PUBLIC ${PROCDRAW_SRC_DIR}
        PRIVATE ${ALSA_INCLUDE_DIRS})

    target_link_libraries(procdraw_alsa_midi
        ${ALSA_LIBRARIES})
endif()

# procdraw_opengl

if(ENABLE_OPENGL)
    add_library(procdraw_opengl
        procdraw/opengl/bitmap_text_renderer.cpp
        procdraw/opengl/bitmap_text_renderer.h
        procdraw/opengl/camera.cpp
        procdraw/opengl/camera.h
        procdraw/opengl/console.cpp
        procdraw/opengl/console.h
        procdraw/opengl/console_line.h
        procdraw/opengl/font_config.h
        procdraw/opengl/frame_counter.cpp
        procdraw/opengl/frame_counter.h
        procdraw/opengl/freetype_font_loader.cpp
        procdraw/opengl/freetype_font_loader.h
        procdraw/opengl/glrenderer.cpp
        procdraw/opengl/glrenderer.h
        procdraw/opengl/gl_util.cpp
        procdraw/opengl/gl_util.h
        procdraw/opengl/procdraw_app.cpp
        procdraw/opengl/procdraw_app.h
        procdraw/opengl/procdraw_app_lisp.cpp
        procdraw/opengl/procdraw_app_lisp.h
        procdraw/opengl/sdl_util.cpp
        procdraw/opengl/sdl_util.h
        procdraw/opengl/vertex.h)

    target_include_directories(procdraw_opengl
        PUBLIC ${PROCDRAW_SRC_DIR}
        PUBLIC ${SDL2_INCLUDE_DIRS}
        PRIVATE ${OPENGL_INCLUDE_DIRS})

    target_link_libraries(procdraw_opengl
        procdraw_lib
        gl3w
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARIES})
endif()

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

if(ENABLE_OPENGL)
    target_link_libraries(procdraw_main procdraw_opengl)
endif()

if(ENABLE_ALSA_MIDI)
    target_link_libraries(procdraw_main procdraw_alsa_midi)
endif()

set_target_properties(procdraw_main
    PROPERTIES OUTPUT_NAME procdraw)

if(WIN32)
    add_custom_command(TARGET procdraw_main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PROCDRAW_SDL2_DLL}
            $<TARGET_FILE_DIR:procdraw_main>)
endif()
