#include "PiperMidiAdapter.h"
#include "constants.h"

PiperMidi::PiperMidiMessage PiperMidiAdapter::buffer[MIDI_RCV_BUFFER_SIZE];
int PiperMidiAdapter::bufferHead = -1;

PiperMidiAdapter::PiperMidiAdapter() {}

void PiperMidiAdapter::loop() {
  // Read from the buffer and send to serial port (rs485)
  while (bufferHead >= 0) {
    PiperMidi::PiperMidiMessage message = buffer[bufferHead--];
    uint8_t packedMessage[2];
    message.pack(packedMessage);
    Serial1.write(packedMessage, 2);
  }
}

void PiperMidiAdapter::handleNoteOn(byte channel, byte note, byte velocity) {
  if (++bufferHead >= MIDI_RCV_BUFFER_SIZE) {
    // buffer overflow, turn neopixel yellow and skip the message
    return;
  }

  // create a PiperMidiMessage and add it to the buffer
  buffer[bufferHead] = PiperMidi::PiperMidiMessage{
      .columnNumber = channel,
      .status = PiperMidi::PiperMidiMessageType::NoteOn,
      .number = note,
  };
}

void PiperMidiAdapter::handleNoteOff(byte channel, byte note, byte velocity) {
  if (++bufferHead >= MIDI_RCV_BUFFER_SIZE) {
    // buffer overflow, turn neopixel yellow and skip the message
    return;
  }

  // create a PiperMidiMessage and add it to the buffer
  buffer[bufferHead] = PiperMidi::PiperMidiMessage{
      .columnNumber = channel,
      .status = PiperMidi::PiperMidiMessageType::NoteOff,
      .number = note,
  };
}