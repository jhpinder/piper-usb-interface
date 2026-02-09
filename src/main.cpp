#include "PiperMidiAdapter.h"
#include "PiperUsbInterface.h"

Adafruit_USBD_MIDI usbMidi;
PiperUsbInterface piperUsbInterface(usbMidi);
PiperMidiAdapter piperMidiAdapter;

MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usbMidi, MIDI);

void setup() {
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