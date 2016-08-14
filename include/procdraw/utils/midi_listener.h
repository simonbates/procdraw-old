#pragma once

namespace procdraw {

    class MidiListener {
    public:
        virtual void OnMidiControllerInput(unsigned int channel, unsigned int controller, int value) = 0;
        virtual ~MidiListener() { }
    };

}
