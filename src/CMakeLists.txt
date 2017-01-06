set(PROCDRAW_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# procdraw_core

add_library(procdraw_core
    procdraw/core/interpreter/lisp_functions.cpp
    procdraw/core/interpreter/lisp_functions.h
    procdraw/core/interpreter/lisp_interpreter.cpp
    procdraw/core/interpreter/lisp_interpreter.h
    procdraw/core/interpreter/lisp_memory.cpp
    procdraw/core/interpreter/lisp_memory.h
    procdraw/core/interpreter/lisp_reader.cpp
    procdraw/core/interpreter/lisp_reader.h
    procdraw/core/interpreter/prettyprinter.cpp
    procdraw/core/interpreter/prettyprinter.h
    procdraw/core/interpreter/signals.cpp
    procdraw/core/interpreter/signals.h
    procdraw/core/bitmap_font_metrics.cpp
    procdraw/core/bitmap_font_metrics.h
    procdraw/core/bitmap_glyph_metrics.h
    procdraw/core/color.cpp
    procdraw/core/color.h
    procdraw/core/command_processor.h
    procdraw/core/line_buffer.cpp
    procdraw/core/line_buffer.h
    procdraw/core/lsystem.cpp
    procdraw/core/lsystem.h
    procdraw/core/math.cpp
    procdraw/core/math.h
    procdraw/core/midi_listener.h
    procdraw/core/simple_moving_average.h
    procdraw/core/text_layout_engine.h
    procdraw/core/text_layout.h)

target_include_directories(procdraw_core
    PUBLIC ${PROCDRAW_SRC_DIR}
    PUBLIC ${GLM_DIR})

target_link_libraries(procdraw_core
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
        procdraw_core
        gl3w
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARIES})
endif()

# procdraw_main

add_executable(procdraw_main
    procdraw/main/main.cpp)

target_include_directories(procdraw_main
    PRIVATE ${PROCDRAW_SRC_DIR})

target_link_libraries(procdraw_main
    procdraw_core)

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

# procdraw_core_tests

get_filename_component(FUNCTION_SPEC_CHECKER_TESTDATA_DIR "procdraw/core_tests/testdata" ABSOLUTE)
get_filename_component(FUNCTION_SPECS_DIR "${PROJECT_SOURCE_DIR}/Documentation/functions" ABSOLUTE)

add_definitions(-DPROCDRAW_FUNCTION_SPEC_CHECKER_TESTDATA_DIR="${FUNCTION_SPEC_CHECKER_TESTDATA_DIR}")
add_definitions(-DPROCDRAW_FUNCTION_SPECS_DIR="${FUNCTION_SPECS_DIR}")

add_executable(procdraw_core_tests
    procdraw/core_tests/interpreter/lisp_interpreter_test.cpp
    procdraw/core_tests/interpreter/lisp_reader_test.cpp
    procdraw/core_tests/interpreter/prettyprinter_test.cpp
    procdraw/core_tests/interpreter/signals_test.cpp
    procdraw/core_tests/color_test.cpp
    procdraw/core_tests/function_spec_checker.cpp
    procdraw/core_tests/function_spec_checker.h
    procdraw/core_tests/function_spec_checker_test.cpp
    procdraw/core_tests/function_specs_test.cpp
    procdraw/core_tests/line_buffer_test.cpp
    procdraw/core_tests/lsystem_test.cpp
    procdraw/core_tests/math_test.cpp
    procdraw/core_tests/simple_moving_average_test.cpp
    procdraw/core_tests/text_layout_engine_test.cpp
    procdraw/core_tests/text_layout_test.cpp)

target_include_directories(procdraw_core_tests
    PRIVATE ${PROCDRAW_SRC_DIR})

target_link_libraries(procdraw_core_tests
    procdraw_core
    gtest
    gtest_main
    tinyxml2
    ${CMAKE_THREAD_LIBS_INIT})

add_custom_target(check
    COMMAND procdraw_core_tests
    DEPENDS procdraw_core_tests)
