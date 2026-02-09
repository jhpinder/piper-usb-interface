#include "PiperMidiAdapter.h"
#include "constants.h"

PiperMidi::PiperMidiMessage PiperMidiAdapter::buffer[MIDI_RCV_BUFFER_SIZE];
volatile int PiperMidiAdapter::bufferHead = 0;
int PiperMidiAdapter::bufferTail = 0;

PiperMidiAdapter::PiperMidiAdapter() {}

void PiperMidiAdapter::loop() {
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
    PiperMidi::PiperMidiMessage& message = buffer[tempBufferTail];
    message.pack(outputBatch + batchCount * 2); // use 2 until we have a packing struct
    batchCount++;
    bufferTail = tempBufferTail;
  }
  Serial1.write(outputBatch, batchCount * 2); // use 2 until we have a packing struct
}

void PiperMidiAdapter::handleNoteOn(byte channel, byte note, byte velocity) {
  int nextBufferHead = (bufferHead + 1) & (MIDI_RCV_BUFFER_SIZE - 1);
  if (nextBufferHead == bufferTail) {
    // buffer overflow, turn neopixel yellow and skip the message
    return;
  }

  // create a PiperMidiMessage and add it to the buffer
  buffer[nextBufferHead] = PiperMidi::PiperMidiMessage{
      .columnNumber = channel,
      .status = PiperMidi::PiperMidiMessageType::NoteOn,
      .number = note,
  };
  bufferHead = nextBufferHead;
}

void PiperMidiAdapter::handleNoteOff(byte channel, byte note, byte velocity) {
  int nextBufferHead = (bufferHead + 1) & (MIDI_RCV_BUFFER_SIZE - 1);
  if (nextBufferHead == bufferTail) {
    // buffer overflow, turn neopixel yellow and skip the message
    return;
  }

  // create a PiperMidiMessage and add it to the buffer
  buffer[nextBufferHead] = PiperMidi::PiperMidiMessage{
      .columnNumber = channel,
      .status = PiperMidi::PiperMidiMessageType::NoteOff,
      .number = note,
  };
  bufferHead = nextBufferHead;
}