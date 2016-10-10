set(PROCDRAW_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# procdraw library

add_library(procdraw_lib
    procdraw/graphics/bitmap_text_renderer.cpp
    procdraw/graphics/camera.cpp
    procdraw/graphics/freetype_font_loader.cpp
    procdraw/graphics/glrenderer.cpp
    procdraw/graphics/gl_util.cpp
    procdraw/graphics/sdl_util.cpp
    procdraw/interpreter/lisp_functions.cpp
    procdraw/interpreter/lisp_interpreter.cpp
    procdraw/interpreter/lisp_memory.cpp
    procdraw/interpreter/lisp_reader.cpp
    procdraw/interpreter/prettyprinter.cpp
    procdraw/interpreter/signals.cpp
    procdraw/color.cpp
    procdraw/console.cpp
    procdraw/font_metrics.cpp
    procdraw/frame_counter.cpp
    procdraw/line_buffer.cpp
    procdraw/lsystem.cpp
    procdraw/utils.cpp)

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

set_target_properties(procdraw_lib
    PROPERTIES OUTPUT_NAME procdraw)

# procdraw_alsa_midi

if(PROCDRAW_ENABLE_ALSA_MIDI)
    add_library(procdraw_alsa_midi
        procdraw_alsa_midi/alsa_midi_client.cpp)

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
    procdraw_app/procdraw_app_lisp.cpp)

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
