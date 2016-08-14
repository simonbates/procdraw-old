#pragma once

#include "procdraw/utils/midi_listener.h"
#include <alsa/asoundlib.h>

namespace procdraw {

    class AlsaMidiClient {
    public:
        AlsaMidiClient();
        void Poll(MidiListener *listener);
    private:
        snd_seq_t *seq_;
        int portid_;
    };

}
