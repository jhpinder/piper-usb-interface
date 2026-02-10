#include "PiperMidiAdapter.h"
#include "constants.h"

template <uint8_t PortID> PiperMidi::PiperMidiMessage PiperMidiAdapter<PortID>::rcvBuffer[MIDI_RCV_BUFFER_SIZE];
template <uint8_t PortID> volatile int PiperMidiAdapter<PortID>::bufferHead = 0;
template <uint8_t PortID> int PiperMidiAdapter<PortID>::bufferTail = 0;

template <uint8_t PortID> PiperMidiAdapter<PortID>::PiperMidiAdapter() {}

template <uint8_t PortID> void PiperMidiAdapter<PortID>::loop() {
  if (Serial1.availableForWrite() < (PIPER_BATCH_SIZE * 2))
    return;
  if (bufferHead == bufferTail) {
    // buffer is empty, nothing to send
    return;
  }

  int tempBufferHead = bufferHead;
  int tempBufferTail = bufferTail;

  uint8_t outputBatch[PIPER_BATCH_SIZE * 2]; // use 2 until we have a packing struct
  int batchCount = 0;

  // Read from the buffer and send to serial port (rs485)
  while (tempBufferHead != tempBufferTail && batchCount < PIPER_BATCH_SIZE) {
    tempBufferTail = (tempBufferTail + 1) & (MIDI_RCV_BUFFER_SIZE - 1);
    PiperMidi::PiperMidiMessage& message = rcvBuffer[tempBufferTail];
    message.pack(outputBatch + batchCount * 2); // use 2 until we have a packing struct
    batchCount++;
    bufferTail = tempBufferTail;
  }
  Serial1.write(outputBatch, batchCount * 2); // use 2 until we have a packing struct
}

template <uint8_t PortID> void PiperMidiAdapter<PortID>::handleNoteOn(byte channel, byte note, byte velocity) {
  int nextBufferHead = (bufferHead + 1) & (MIDI_RCV_BUFFER_SIZE - 1);
  if (nextBufferHead == bufferTail) {
    // buffer overflow, turn neopixel yellow and skip the message
    return;
  }

  // create a PiperMidiMessage and add it to the buffer
  rcvBuffer[nextBufferHead] = PiperMidi::PiperMidiMessage{
      .columnNumber = channel,
      .status = PiperMidi::PiperMidiMessageType::NoteOn,
      .number = note,
  };
  bufferHead = nextBufferHead;
}

template <uint8_t PortID> void PiperMidiAdapter<PortID>::handleNoteOff(byte channel, byte note, byte velocity) {
  int nextBufferHead = (bufferHead + 1) & (MIDI_RCV_BUFFER_SIZE - 1);
  if (nextBufferHead == bufferTail) {
    // buffer overflow, turn neopixel yellow and skip the message
    return;
  }

  // create a PiperMidiMessage and add it to the buffer
  rcvBuffer[nextBufferHead] = PiperMidi::PiperMidiMessage{
      .columnNumber = channel,
      .status = PiperMidi::PiperMidiMessageType::NoteOff,
      .number = note,
  };
  bufferHead = nextBufferHead;
}

// four ports for now, will let us control 16,384 output points
template class PiperMidiAdapter<0>;
template class PiperMidiAdapter<1>;
template class PiperMidiAdapter<2>;
template class PiperMidiAdapter<3>;