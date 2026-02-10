#include "constants.h"
#include <MIDI.h>

template <uint8_t PortID> class PiperMidiAdapter {
public:
  PiperMidiAdapter();
  static void loop();
  static void handleNoteOn(byte channel, byte note, byte velocity);
  static void handleNoteOff(byte channel, byte note, byte velocity);

private:
  static PiperMidi::PiperMidiMessage rcvBuffer[MIDI_RCV_BUFFER_SIZE];
  static PiperMidi::PackedPiperMidiMessage outputBatchBuffer[PIPER_BATCH_SIZE];
  static volatile uint32_t bufferHead;
  static volatile uint32_t bufferTail;
};