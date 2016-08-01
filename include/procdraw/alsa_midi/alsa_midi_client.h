#pragma once

#include <alsa/asoundlib.h>

namespace procdraw {

    class MidiListener {
    public:
        virtual void OnMidiControllerInput(unsigned int channel, unsigned int controller, int value) = 0;
        virtual ~MidiListener() { }
    };

    class AlsaMidiClient {
    public:
        AlsaMidiClient();
        void Poll(MidiListener *listener);
    private:
        snd_seq_t *seq_;
        int portid_;
    };

}
