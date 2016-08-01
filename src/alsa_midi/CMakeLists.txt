add_library(alsa_midi
    alsa_midi_client.cpp)

target_include_directories(alsa_midi
    PUBLIC ${PROCDRAW_INCLUDE_DIR}
    PRIVATE ${ALSA_INCLUDE_DIRS})

target_link_libraries(alsa_midi
    ${ALSA_LIBRARIES})
