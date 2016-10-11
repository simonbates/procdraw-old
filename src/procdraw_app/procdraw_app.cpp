#include "procdraw_app/procdraw_app.h"
#include "procdraw_core/interpreter/signals.h"
#include "procdraw_app/procdraw_app_lisp.h"
#include PROCDRAW_SDL2_H
#include <iostream>
#include <string>
#include <vector>

static const int numMidiControllers = 33;

namespace procdraw {

ProcdrawApp::ProcdrawApp()
{
  S_EVENT = L_.SymbolRef("event");
  S_LOG_MIDI = L_.SymbolRef("log-midi");
  S_SHOW_REPL = L_.SymbolRef("show-repl");
  S_OUT = L_.SymbolRef("out");

  midiChannelOneControllers_ =
    std::vector<LispObjectPtr>(numMidiControllers, L_.Nil);

  RegisterSignals(&L_);
  RegisterProcdrawAppFunctions(this, &L_);
  MakeKeySignals();
  MakeMidiSignals();

  InitProg();

  L_.Set(S_LOG_MIDI, L_.False, L_.Nil);
  L_.Set(S_SHOW_REPL, L_.True, L_.Nil);

  console_ = std::unique_ptr<Console>(
    new Console(&renderer_, this, renderer_.Width(), renderer_.Height()));

  console_->Println("Procdraw");
  console_->Println("");
  console_->Println("Press ESC to show and hide the REPL");
  console_->Println("");
}

int
ProcdrawApp::MainLoop()
{
  bool quit = false;

  SDL_Event e;
  while (!quit) {
    L_.Call("clear-stepped-signals");
    auto showReplVal = L_.BoolVal(IsShowRepl());
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT:
          quit = true;
          break;
        case SDL_KEYDOWN:
          if (e.key.keysym.sym == SDLK_ESCAPE) {
            if (e.key.repeat == 0) {
              L_.Set(S_SHOW_REPL, L_.Not(IsShowRepl()), L_.Nil);
            }
          } else {
            if (showReplVal) {
              console_->ProcessKey(&(e.key));
            } else {
              if ((e.key.keysym.sym == SDLK_SPACE) && (e.key.repeat == 0)) {
                PutSlot(&L_, keySpaceSignal_, S_EVENT, L_.True);
              }
            }
          }
          break;
        case SDL_TEXTINPUT:
          if (showReplVal) {
            console_->InputText(e.text.text);
          }
          break;
      }
    }
#ifdef PROCDRAW_ENABLE_ALSA_MIDI
    midiClient_.Poll(this);
#endif
    L_.Call("draw");
    if (showReplVal) {
      console_->Draw();
    }
    renderer_.DoSwap();
    frameCounter_.RecordFrame();
  }
  return 0;
}

std::string
ProcdrawApp::DoCommand(const std::string& cmd)
{
  std::string val;
  try {
    // TODO: Use actual console width, rather than hardcoding a value
    val = L_.PrettyPrintToString(L_.Eval(L_.Read(cmd)), 40);
  } catch (std::exception e) {
    val = e.what();
  }
  return val;
}

BalancedState
ProcdrawApp::CheckCommand(const std::string& cmd)
{
  return L_.CheckBalanced(cmd);
}

double
ProcdrawApp::FramesPerSecond()
{
  return frameCounter_.GetFramesPerSecond();
}

void
ProcdrawApp::Message(const std::string& msg)
{
  std::cout << msg << std::endl;
}

void
ProcdrawApp::OnMidiControllerInput(unsigned int channel,
                                   unsigned int controller, int value)
{
  if (channel == 1 && controller < numMidiControllers) {
    PutSlot(&L_, midiChannelOneControllers_[controller], S_OUT,
            L_.MakeNumber(value / 128.0));
  }

  if (L_.BoolVal(L_.SymbolValue(S_LOG_MIDI))) {
    std::ostringstream s;
    s << "Control Change ch " << channel;
    s << " controller " << controller;
    s << " value " << value;
    Message(s.str());
  }
}

LispObjectPtr
ProcdrawApp::IsShowRepl()
{
  return L_.SymbolValue(S_SHOW_REPL);
}

void
ProcdrawApp::MakeKeySignals()
{
  keySpaceSignal_ = MakeEventRelaySignal(&L_);
  L_.Set(L_.SymbolRef("key-space"), keySpaceSignal_, L_.Nil);
}

void
ProcdrawApp::MakeMidiSignals()
{
  for (int i = 0; i < numMidiControllers; ++i) {
    midiChannelOneControllers_[i] = MakeMidiSignal(1, i);
  }
}

LispObjectPtr
ProcdrawApp::MakeMidiSignal(int channel, int controller)
{
  auto signal = MakeSignal(&L_, L_.Nil);
  PutSlot(&L_, signal, S_OUT, L_.MakeNumber(0));
  std::ostringstream name;
  name << "midicc-" << channel << "-" << controller;
  L_.Set(L_.SymbolRef(name.str()), signal, L_.Nil);
  return signal;
}

void
ProcdrawApp::InitProg()
{
  const std::string prog =
    "(progn                                                           "
    "  (setq y-angle (saw))                                           "
    "  (put-slot y-angle 'freq (/ (* 37 60)))                         "
    "  (setq z-angle (saw))                                           "
    "  (put-slot z-angle 'freq (/ (* 181 60)))                        "
    "  (def draw ()                                                   "
    "    (background 200 (/ 6 10) (/ 9 10))                           "
    "    (rotate-z (sigval z-angle))                                  "
    "    (rotate-y (sigval y-angle))                                  "
    "    (translate 6 0 0)                                            "
    "    (rotate-y (sigval y-angle))                                  "
    "    (color 7 (/ 7 10) (/ 7 10))                                  "
    "    (tetrahedron)))                                              ";

  L_.Eval(L_.Read(prog));
}
}
