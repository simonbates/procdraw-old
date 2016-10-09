set(PROCDRAW_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# procdraw_alsa_midi

if(PROCDRAW_ENABLE_ALSA_MIDI)
    add_library(procdraw_alsa_midi
        procdraw/alsa_midi/alsa_midi_client.cpp)

    target_include_directories(procdraw_alsa_midi
        PUBLIC ${PROCDRAW_SRC_DIR}
        PRIVATE ${ALSA_INCLUDE_DIRS})

    target_link_libraries(procdraw_alsa_midi
        ${ALSA_LIBRARIES})
endif()

# procdraw_test

add_library(procdraw_test
    procdraw/test/function_spec_checker.cpp)

target_include_directories(procdraw_test
    PUBLIC ${PROCDRAW_SRC_DIR})

target_link_libraries(procdraw_test
    tinyxml2)

# procdraw_lib

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
    procdraw/procdraw_app.cpp
    procdraw/procdraw_app_lisp.cpp
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

# procdraw executable

add_executable(procdraw
    main.cpp)

target_include_directories(procdraw
    PRIVATE ${PROCDRAW_SRC_DIR})

target_link_libraries(procdraw
    procdraw_lib)

if(PROCDRAW_ENABLE_ALSA_MIDI)
    target_link_libraries(procdraw procdraw_alsa_midi)
endif()

if(WIN32)
    add_custom_command(TARGET procdraw POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PROCDRAW_SDL2_DLL}
            $<TARGET_FILE_DIR:procdraw>)
endif()
