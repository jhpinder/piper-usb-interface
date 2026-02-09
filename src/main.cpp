#include "PiperMidiAdapter.h"
#include "PiperUsbInterface.h"

Adafruit_USBD_MIDI usbMidi;
PiperUsbInterface piperUsbInterface(usbMidi);
PiperMidiAdapter piperMidiAdapter;

MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usbMidi, MIDI);

void setup() {
  // MIDI_RCV_BUFFER_SIZE must be a power of 2
  static_assert((MIDI_RCV_BUFFER_SIZE & (MIDI_RCV_BUFFER_SIZE - 1)) == 0);
  piperUsbInterface.begin();
  Serial1.setTX(RS485_TX_PIN);
  Serial1.begin(RS485_BAUD_RATE);
  piperUsbInterface.reMount();
  MIDI.setHandleNoteOn(PiperMidiAdapter::handleNoteOn);
  MIDI.setHandleNoteOff(PiperMidiAdapter::handleNoteOff);
}

void loop() {
  piperUsbInterface.loop();
  MIDI.read();
  piperMidiAdapter.loop();
}