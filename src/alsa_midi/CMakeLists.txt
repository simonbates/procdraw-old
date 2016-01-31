include_directories(${ALSA_INCLUDE_DIRS})

add_library(alsa_midi
            alsa_midi_client.cpp)
