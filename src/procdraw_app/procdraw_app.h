#pragma once

#include "procdraw_core/graphics/glrenderer.h"
#include "procdraw_core/interpreter/lisp_interpreter.h"
#include "procdraw_core/command_processor.h"
#include "procdraw_core/console.h"
#include "procdraw_core/frame_counter.h"
#include "procdraw_core/midi_listener.h"

#ifdef PROCDRAW_ENABLE_ALSA_MIDI
#include "procdraw_alsa_midi/alsa_midi_client.h"
#endif

#include <memory>
#include <string>
#include <vector>

namespace procdraw {

class ProcdrawApp : public CommandProcessor, MidiListener
{
public:
  ProcdrawApp();
  inline GlRenderer* Renderer() { return &renderer_; }
  int MainLoop();
  std::string DoCommand(const std::string& cmd);
  BalancedState CheckCommand(const std::string& cmd);
  double FramesPerSecond();
  void Message(const std::string& msg);
  void OnMidiControllerInput(unsigned int channel, unsigned int controller,
                             int value);

private:
  GlRenderer renderer_;
  LispInterpreter L_;
  LispObjectPtr S_EVENT;
  LispObjectPtr S_LOG_MIDI;
  LispObjectPtr S_SHOW_REPL;
  LispObjectPtr S_OUT;
  std::unique_ptr<Console> console_;
  FrameCounter frameCounter_;
#ifdef PROCDRAW_ENABLE_ALSA_MIDI
  AlsaMidiClient midiClient_;
#endif
  LispObjectPtr keySpaceSignal_;
  std::vector<LispObjectPtr> midiChannelOneControllers_;
  LispObjectPtr IsShowRepl();
  void MakeKeySignals();
  void MakeMidiSignals();
  LispObjectPtr MakeMidiSignal(int channel, int controller);
  void InitProg();
};
}
