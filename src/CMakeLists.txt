set(PROCDRAW_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# procdraw_core

add_library(procdraw_core
    procdraw_core/graphics/bitmap_text_renderer.cpp
    procdraw_core/graphics/bitmap_text_renderer.h
    procdraw_core/graphics/camera.cpp
    procdraw_core/graphics/camera.h
    procdraw_core/graphics/font_config.h
    procdraw_core/graphics/freetype_font_loader.cpp
    procdraw_core/graphics/freetype_font_loader.h
    procdraw_core/graphics/glrenderer.cpp
    procdraw_core/graphics/glrenderer.h
    procdraw_core/graphics/gl_util.cpp
    procdraw_core/graphics/gl_util.h
    procdraw_core/graphics/sdl_util.cpp
    procdraw_core/graphics/sdl_util.h
    procdraw_core/graphics/vertex.h
    procdraw_core/interpreter/lisp_functions.cpp
    procdraw_core/interpreter/lisp_functions.h
    procdraw_core/interpreter/lisp_interpreter.cpp
    procdraw_core/interpreter/lisp_interpreter.h
    procdraw_core/interpreter/lisp_memory.cpp
    procdraw_core/interpreter/lisp_memory.h
    procdraw_core/interpreter/lisp_reader.cpp
    procdraw_core/interpreter/lisp_reader.h
    procdraw_core/interpreter/prettyprinter.cpp
    procdraw_core/interpreter/prettyprinter.h
    procdraw_core/interpreter/signals.cpp
    procdraw_core/interpreter/signals.h
    procdraw_core/color.cpp
    procdraw_core/color.h
    procdraw_core/command_processor.h
    procdraw_core/console.cpp
    procdraw_core/console.h
    procdraw_core/font_metrics.cpp
    procdraw_core/font_metrics.h
    procdraw_core/frame_counter.cpp
    procdraw_core/frame_counter.h
    procdraw_core/line_buffer.cpp
    procdraw_core/line_buffer.h
    procdraw_core/lsystem.cpp
    procdraw_core/lsystem.h
    procdraw_core/math.cpp
    procdraw_core/math.h
    procdraw_core/midi_listener.h
    procdraw_core/text_layout_engine.h
    procdraw_core/text_layout.h)

target_include_directories(procdraw_core
    PUBLIC ${PROCDRAW_SRC_DIR}
    PUBLIC ${GLM_DIR}
    PUBLIC ${SDL2_INCLUDE_DIRS}
    PRIVATE ${OPENGL_INCLUDE_DIRS})

target_link_libraries(procdraw_core
    freetype2
    gl3w
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARIES})

# procdraw_alsa_midi

if(PROCDRAW_ENABLE_ALSA_MIDI)
    add_library(procdraw_alsa_midi
        procdraw_alsa_midi/alsa_midi_client.cpp
        procdraw_alsa_midi/alsa_midi_client.h)

    target_include_directories(procdraw_alsa_midi
        PUBLIC ${PROCDRAW_SRC_DIR}
        PRIVATE ${ALSA_INCLUDE_DIRS})

    target_link_libraries(procdraw_alsa_midi
        ${ALSA_LIBRARIES})
endif()

# procdraw_app

add_executable(procdraw_app
    procdraw_app/main.cpp
    procdraw_app/procdraw_app.cpp
    procdraw_app/procdraw_app.h
    procdraw_app/procdraw_app_lisp.cpp
    procdraw_app/procdraw_app_lisp.h)

target_include_directories(procdraw_app
    PRIVATE ${PROCDRAW_SRC_DIR})

target_link_libraries(procdraw_app
    procdraw_core)

if(PROCDRAW_ENABLE_ALSA_MIDI)
    target_link_libraries(procdraw_app procdraw_alsa_midi)
endif()

set_target_properties(procdraw_app
    PROPERTIES OUTPUT_NAME procdraw)

if(WIN32)
    add_custom_command(TARGET procdraw_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PROCDRAW_SDL2_DLL}
            $<TARGET_FILE_DIR:procdraw_app>)
endif()

# procdraw_core_tests

get_filename_component(FUNCTION_SPEC_CHECKER_TESTDATA_DIR "procdraw_core_tests/testdata" ABSOLUTE)
get_filename_component(FUNCTION_SPECS_DIR "${PROJECT_SOURCE_DIR}/Documentation/functions" ABSOLUTE)

add_definitions(-DPROCDRAW_FUNCTION_SPEC_CHECKER_TESTDATA_DIR="${FUNCTION_SPEC_CHECKER_TESTDATA_DIR}")
add_definitions(-DPROCDRAW_FUNCTION_SPECS_DIR="${FUNCTION_SPECS_DIR}")

add_executable(procdraw_core_tests
    procdraw_core_tests/interpreter/lisp_interpreter_test.cpp
    procdraw_core_tests/interpreter/lisp_reader_test.cpp
    procdraw_core_tests/interpreter/prettyprinter_test.cpp
    procdraw_core_tests/interpreter/signals_test.cpp
    procdraw_core_tests/color_test.cpp
    procdraw_core_tests/function_spec_checker.cpp
    procdraw_core_tests/function_spec_checker.h
    procdraw_core_tests/function_spec_checker_test.cpp
    procdraw_core_tests/function_specs_test.cpp
    procdraw_core_tests/line_buffer_test.cpp
    procdraw_core_tests/lsystem_test.cpp
    procdraw_core_tests/math_test.cpp
    procdraw_core_tests/text_layout_engine_test.cpp
    procdraw_core_tests/text_layout_test.cpp)

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
