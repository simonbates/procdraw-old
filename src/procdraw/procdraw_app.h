#pragma once

#include "glrenderer.h"
#include "lisp_interpreter.h"
#include "cli.h"
#include "console.h"
#include "command_processor.h"
#include "frame_counter.h"
#include "alsa_midi_client.h"
#include <memory>
#include <string>
#include <vector>

namespace procdraw {

    class ProcdrawApp : public CommandProcessor, MidiListener {
    public:
        ProcdrawApp();
        inline GlRenderer* Renderer()
        {
            return &renderer_;
        }
        int MainLoop();
        std::string DoCommand(const std::string &cmd);
        BalancedState CheckCommand(const std::string &cmd);
        double FramesPerSecond();
        void Message(const std::string &msg);
        void OnMidiControllerInput(unsigned int channel, unsigned int controller, int value);
    private:
        GlRenderer renderer_;
        LispInterpreter L_;
        LispObjectPtr S_EVENT;
        LispObjectPtr S_LOG_MIDI;
        LispObjectPtr S_SHOW_REPL;
        LispObjectPtr S_OUT;
        std::unique_ptr<CLI> cli_;
        std::unique_ptr<Console> console_;
        FrameCounter frameCounter_;
        AlsaMidiClient midiClient_;
        LispObjectPtr keySpaceSignal_;
        std::vector<LispObjectPtr> midiChannelOneControllers_;
        LispObjectPtr IsShowRepl();
        void MakeKeySignals();
        void MakeMidiSignals();
        LispObjectPtr MakeMidiSignal(int channel, int controller);
        void InitProg();
    };

}
