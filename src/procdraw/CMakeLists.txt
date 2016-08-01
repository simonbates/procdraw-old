add_executable(procdraw
    cli.cpp
    console.cpp
    frame_counter.cpp
    main.cpp
    procdraw_app.cpp
    procdraw_app_lisp.cpp)

target_include_directories(procdraw
    PRIVATE ${PROCDRAW_INCLUDE_DIR}
    PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(procdraw
    interpreter
    graphics
    alsa_midi
    readline)
