#include "PiperMidiAdapter.h"
#include "constants.h"

template <uint8_t PortID> PiperMidi::PiperMidiMessage PiperMidiAdapter<PortID>::rcvBuffer[MIDI_RCV_BUFFER_SIZE];
template <uint8_t PortID> volatile uint32_t PiperMidiAdapter<PortID>::bufferHead = 0;
template <uint8_t PortID> volatile uint32_t PiperMidiAdapter<PortID>::bufferTail = 0;

template <uint8_t PortID> PiperMidiAdapter<PortID>::PiperMidiAdapter() { serialPort.begin(RS485_BAUD_RATE); }

template <uint8_t PortID>
PiperMidi::PackedPiperMidiMessage PiperMidiAdapter<PortID>::outputBatchBuffer[PIPER_BATCH_SIZE];

template <uint8_t PortID> SerialPIO PiperMidiAdapter<PortID>::serialPort(PortID, -1);

template <uint8_t PortID> void PiperMidiAdapter<PortID>::begin() { serialPort.begin(RS485_BAUD_RATE); }

template <uint8_t PortID> void PiperMidiAdapter<PortID>::loop() {
  if (serialPort.availableForWrite() < (PIPER_BATCH_SIZE * sizeof(PiperMidi::PackedPiperMidiMessage)))
    return;
  if (bufferHead == bufferTail) {
    // buffer is empty, nothing to send
    return;
  }

  // set batch buffer pointers and counters
  uint32_t batchBufferHead = bufferHead;
  uint32_t batchBufferTail = bufferTail;

  int batchCount = 0;

  // read from the buffer and pack
  while (batchBufferHead != batchBufferTail && batchCount < PIPER_BATCH_SIZE) {
    batchBufferTail = (batchBufferTail + 1) & (MIDI_RCV_BUFFER_SIZE - 1);
    rcvBuffer[batchBufferTail].pack(&outputBatchBuffer[batchCount++]);
  }

  bufferTail = batchBufferTail;
  serialPort.write(reinterpret_cast<const uint8_t*>(outputBatchBuffer),
                   batchCount * sizeof(PiperMidi::PackedPiperMidiMessage));
}

template <uint8_t PortID> void PiperMidiAdapter<PortID>::handleNoteOn(byte channel, byte note, byte velocity) {
  int nextBufferHead = (bufferHead + 1) & (MIDI_RCV_BUFFER_SIZE - 1);
  if (nextBufferHead == bufferTail) {
    // buffer overflow, turn neopixel yellow and skip the message
    return;
  }

  // reference the next PiperMidiMessage in the buffer and populate it with the incoming MIDI data
  PiperMidi::PiperMidiMessage& message = rcvBuffer[nextBufferHead];
  message.columnNumber = channel;
  message.status = PiperMidi::PiperMidiMessageType::NoteOn;
  message.number = note;

  __dmb(); // ensure the message is fully written before updating the head pointer
  bufferHead = nextBufferHead;
}

template <uint8_t PortID> void PiperMidiAdapter<PortID>::handleNoteOff(byte channel, byte note, byte velocity) {
  int nextBufferHead = (bufferHead + 1) & (MIDI_RCV_BUFFER_SIZE - 1);
  if (nextBufferHead == bufferTail) {
    // buffer overflow, turn neopixel yellow and skip the message
    return;
  }

  // reference the next PiperMidiMessage in the buffer and populate it with the incoming MIDI data
  PiperMidi::PiperMidiMessage& message = rcvBuffer[nextBufferHead];
  message.columnNumber = channel;
  message.status = PiperMidi::PiperMidiMessageType::NoteOff;
  message.number = note;

  __dmb(); // ensure the message is fully written before updating the head pointer
  bufferHead = nextBufferHead;
}

// four ports for now, will let us control 16,384 output points
template class PiperMidiAdapter<0>;
template class PiperMidiAdapter<1>;
template class PiperMidiAdapter<2>;
template class PiperMidiAdapter<3>;