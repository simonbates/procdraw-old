#include "procdraw_app.h"
#include "procdraw_app_lisp.h"
#include "signals.h"
#include <SDL2/SDL.h>
#include <iostream>

static const int numMidiControllers = 33;

namespace procdraw {

    ProcdrawApp::ProcdrawApp()
    {
        S_EVENT = L_.SymbolRef("event");
        S_LOG_MIDI = L_.SymbolRef("log-midi");
        S_SHOW_REPL = L_.SymbolRef("show-repl");
        S_OUT = L_.SymbolRef("out");

        midiChannelOneControllers_ = std::vector<LispObjectPtr>(numMidiControllers, L_.Nil);

        RegisterSignals(&L_);
        RegisterProcdrawAppFunctions(this, &L_);
        MakeKeySignals();
        MakeMidiSignals();

        InitDraw();

        L_.Set(S_LOG_MIDI, L_.False, L_.Nil);
        L_.Set(S_SHOW_REPL, L_.False, L_.Nil);

        cli_ = std::unique_ptr<CLI>(new CLI(this));
    }

    int ProcdrawApp::MainLoop()
    {
        bool quit = false;

        SDL_Event e;
        while (!quit) {
            L_.Call("clear-stepped-signals");
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN) {
                    if ((e.key.keysym.sym == SDLK_ESCAPE) && (e.key.repeat == 0)) {
                        L_.Set(S_SHOW_REPL, L_.Not(ShowRepl()), L_.Nil);
                    }
                    else if ((e.key.keysym.sym == SDLK_SPACE) && (e.key.repeat == 0)) {
                        PutSlot(&L_, keySpaceSignal_, S_EVENT, L_.True);
                    }
                }
            }
            cli_->Poll();
            midiClient_.Poll(this);
            L_.Call("draw");
            if (L_.BoolVal(ShowRepl())) {
                renderer_.Begin2D();
                renderer_.Rect(20, 20, renderer_.Width() - 40, renderer_.Height() - 40);
                renderer_.BeginText();
                renderer_.Text(30, 30, "hello, world");
                renderer_.Begin3D();
            }
            renderer_.DoSwap();
            frameCounter_.RecordFrame();
        }
        return 0;
    }

    std::string ProcdrawApp::DoCommand(const std::string &cmd)
    {
        std::string val;
        try {
            val = L_.PrintToString(L_.Eval(L_.Read(cmd)));
        }
        catch (std::exception e) {
            val = e.what();
        }
        return val;
    }

    BalancedState ProcdrawApp::CheckCommand(const std::string &cmd)
    {
        return L_.CheckBalanced(cmd);
    }

    double ProcdrawApp::FramesPerSecond()
    {
        return frameCounter_.GetFramesPerSecond();
    }

    void ProcdrawApp::Message(const std::string &msg)
    {
        std::cout << msg << std::endl;
    }

    void ProcdrawApp::OnMidiControllerInput(unsigned int channel, unsigned int controller, int value)
    {
        if (channel == 1 && controller < numMidiControllers) {
            PutSlot(&L_, midiChannelOneControllers_[controller], S_OUT, L_.MakeNumber(value / 128.0));
        }

        if (L_.BoolVal(L_.SymbolValue(S_LOG_MIDI))) {
            std::ostringstream s;
            s << "Control Change ch " << channel;
            s << " controller " << controller;
            s << " value " << value;
            Message(s.str());
        }
    }

    LispObjectPtr ProcdrawApp::ShowRepl()
    {
        return L_.SymbolValue(S_SHOW_REPL);
    }

    void ProcdrawApp::MakeKeySignals()
    {
        keySpaceSignal_ = MakeEventRelaySignal(&L_);
        L_.Set(L_.SymbolRef("key-space"), keySpaceSignal_, L_.Nil);
    }

    void ProcdrawApp::MakeMidiSignals()
    {
        for (int i = 0; i < numMidiControllers; ++i) {
            midiChannelOneControllers_[i] = MakeMidiSignal(1, i);
        }
    }

    LispObjectPtr ProcdrawApp::MakeMidiSignal(int channel, int controller)
    {
        auto signal = MakeSignal(&L_, L_.Nil);
        PutSlot(&L_, signal, S_OUT, L_.MakeNumber(0));
        std::ostringstream name;
        name << "midic-" << channel << "-" << controller;
        L_.Set(L_.SymbolRef(name.str()), signal, L_.Nil);
        return signal;
    }

    void ProcdrawApp::InitDraw()
    {
        auto drawFun = L_.Read("(lambda () (background 0 0 0))");
        L_.Set(L_.SymbolRef("draw"), drawFun, L_.Nil);
    }

}
