#include "procdraw_alsa_midi/alsa_midi_client.h"
#include <stdexcept>

namespace procdraw {

AlsaMidiClient::AlsaMidiClient()
  : seq_(nullptr)
  , portid_(0)
{
  if (snd_seq_open(&seq_, "default", SND_SEQ_OPEN_INPUT, 0) < 0) {
    throw std::runtime_error("Error opening ALSA sequencer");
  }

  snd_seq_set_client_name(seq_, "Procdraw");

  portid_ = snd_seq_create_simple_port(seq_, "Procdraw Input",
                                       SND_SEQ_PORT_CAP_WRITE |
                                         SND_SEQ_PORT_CAP_SUBS_WRITE,
                                       SND_SEQ_PORT_TYPE_APPLICATION);
  if (portid_ < 0) {
    throw std::runtime_error("Error creating sequencer port");
  }
}

void
AlsaMidiClient::Poll(MidiListener* listener)
{
  snd_seq_event_t* ev;

  while (snd_seq_event_input_pending(seq_, 1) > 0) {
    snd_seq_event_input(seq_, &ev);
    if (ev->type == SND_SEQ_EVENT_CONTROLLER) {
      listener->OnMidiControllerInput(
        static_cast<unsigned int>(ev->data.control.channel) + 1,
        ev->data.control.param, ev->data.control.value);
    }
  }
}
}
