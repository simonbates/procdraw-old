set(PROCDRAW_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# procdraw library

add_library(procdraw_lib
    procdraw_lib/procdraw/graphics/bitmap_text_renderer.cpp
    procdraw_lib/procdraw/graphics/bitmap_text_renderer.h
    procdraw_lib/procdraw/graphics/camera.cpp
    procdraw_lib/procdraw/graphics/camera.h
    procdraw_lib/procdraw/graphics/font_config.h
    procdraw_lib/procdraw/graphics/freetype_font_loader.cpp
    procdraw_lib/procdraw/graphics/freetype_font_loader.h
    procdraw_lib/procdraw/graphics/glrenderer.cpp
    procdraw_lib/procdraw/graphics/glrenderer.h
    procdraw_lib/procdraw/graphics/gl_util.cpp
    procdraw_lib/procdraw/graphics/gl_util.h
    procdraw_lib/procdraw/graphics/sdl_util.cpp
    procdraw_lib/procdraw/graphics/sdl_util.h
    procdraw_lib/procdraw/interpreter/lisp_functions.cpp
    procdraw_lib/procdraw/interpreter/lisp_functions.h
    procdraw_lib/procdraw/interpreter/lisp_interpreter.cpp
    procdraw_lib/procdraw/interpreter/lisp_interpreter.h
    procdraw_lib/procdraw/interpreter/lisp_memory.cpp
    procdraw_lib/procdraw/interpreter/lisp_memory.h
    procdraw_lib/procdraw/interpreter/lisp_reader.cpp
    procdraw_lib/procdraw/interpreter/lisp_reader.h
    procdraw_lib/procdraw/interpreter/prettyprinter.cpp
    procdraw_lib/procdraw/interpreter/prettyprinter.h
    procdraw_lib/procdraw/interpreter/signals.cpp
    procdraw_lib/procdraw/interpreter/signals.h
    procdraw_lib/procdraw/color.cpp
    procdraw_lib/procdraw/color.h
    procdraw_lib/procdraw/command_processor.h
    procdraw_lib/procdraw/console.cpp
    procdraw_lib/procdraw/console.h
    procdraw_lib/procdraw/font_metrics.cpp
    procdraw_lib/procdraw/font_metrics.h
    procdraw_lib/procdraw/frame_counter.cpp
    procdraw_lib/procdraw/frame_counter.h
    procdraw_lib/procdraw/line_buffer.cpp
    procdraw_lib/procdraw/line_buffer.h
    procdraw_lib/procdraw/lsystem.cpp
    procdraw_lib/procdraw/lsystem.h
    procdraw_lib/procdraw/midi_listener.h
    procdraw_lib/procdraw/text_layout_engine.h
    procdraw_lib/procdraw/text_layout.h
    procdraw_lib/procdraw/utils.cpp
    procdraw_lib/procdraw/utils.h)

target_include_directories(procdraw_lib
    PUBLIC ${PROCDRAW_SRC_DIR}/procdraw_lib
    PUBLIC ${GLM_DIR}
    PUBLIC ${SDL2_INCLUDE_DIRS}
    PRIVATE ${OPENGL_INCLUDE_DIRS})

target_link_libraries(procdraw_lib
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
        PUBLIC ${PROCDRAW_SRC_DIR}/procdraw_lib
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
    procdraw_lib)

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

# procdraw_tests

get_filename_component(FUNCTION_SPEC_CHECKER_TESTDATA_DIR "procdraw_tests/testdata" ABSOLUTE)
get_filename_component(FUNCTION_SPECS_DIR "${PROJECT_SOURCE_DIR}/Documentation/functions" ABSOLUTE)

add_definitions(-DPROCDRAW_FUNCTION_SPEC_CHECKER_TESTDATA_DIR="${FUNCTION_SPEC_CHECKER_TESTDATA_DIR}")
add_definitions(-DPROCDRAW_FUNCTION_SPECS_DIR="${FUNCTION_SPECS_DIR}")

add_executable(procdraw_tests
    procdraw_tests/interpreter/prettyprinter_test.cpp
    procdraw_tests/color_test.cpp
    procdraw_tests/function_spec_checker.cpp
    procdraw_tests/function_spec_checker.h
    procdraw_tests/function_spec_checker_test.cpp
    procdraw_tests/function_specs_test.cpp
    procdraw_tests/line_buffer_test.cpp
    procdraw_tests/lsystem_test.cpp
    procdraw_tests/text_layout_engine_test.cpp
    procdraw_tests/text_layout_test.cpp
    procdraw_tests/utils_test.cpp)

target_link_libraries(procdraw_tests
    procdraw_lib
    gtest
    gtest_main
    tinyxml2
    ${CMAKE_THREAD_LIBS_INIT})

add_test(procdraw_tests procdraw_tests)

# Legacy Catch tests

add_executable(procdraw_catch_tests
    procdraw_catch_tests/main.cpp
    procdraw_catch_tests/test_lisp_interpreter.cpp
    procdraw_catch_tests/test_lisp_reader.cpp
    procdraw_catch_tests/test_signals.cpp)

target_include_directories(procdraw_catch_tests
    PRIVATE ${CATCH_INCLUDE_DIR})

target_link_libraries(procdraw_catch_tests
    procdraw_lib)

add_test(procdraw_catch_tests procdraw_catch_tests)
