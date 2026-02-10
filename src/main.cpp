#include "PiperMidiAdapter.h"
#include "PiperUsbInterface.h"

Adafruit_USBD_MIDI usbMidi(4);
PiperUsbInterface piperUsbInterface(usbMidi);

midi::SerialMIDI<Adafruit_USBD_MIDI> serialMIDI0(usbMidi);
midi::SerialMIDI<Adafruit_USBD_MIDI> serialMIDI1(usbMidi);
midi::SerialMIDI<Adafruit_USBD_MIDI> serialMIDI2(usbMidi);
midi::SerialMIDI<Adafruit_USBD_MIDI> serialMIDI3(usbMidi);

midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>> MIDI0(serialMIDI0);
midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>> MIDI1(serialMIDI1);
midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>> MIDI2(serialMIDI2);
midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>> MIDI3(serialMIDI3);

void setup() {
  // MIDI_RCV_BUFFER_SIZE must be a power of 2
  static_assert((MIDI_RCV_BUFFER_SIZE & (MIDI_RCV_BUFFER_SIZE - 1)) == 0);

  piperUsbInterface.begin();
  Serial1.setTX(RS485_TX_PIN);
  Serial1.begin(RS485_BAUD_RATE);
  piperUsbInterface.reMount();

  MIDI0.setHandleNoteOn(PiperMidiAdapter<0>::handleNoteOn);
  MIDI0.setHandleNoteOff(PiperMidiAdapter<0>::handleNoteOff);
  MIDI1.setHandleNoteOn(PiperMidiAdapter<1>::handleNoteOn);
  MIDI1.setHandleNoteOff(PiperMidiAdapter<1>::handleNoteOff);
  MIDI2.setHandleNoteOn(PiperMidiAdapter<2>::handleNoteOn);
  MIDI2.setHandleNoteOff(PiperMidiAdapter<2>::handleNoteOff);
  MIDI3.setHandleNoteOn(PiperMidiAdapter<3>::handleNoteOn);
  MIDI3.setHandleNoteOff(PiperMidiAdapter<3>::handleNoteOff);
}

void loop() {
  piperUsbInterface.loop();

  MIDI0.read();
  MIDI1.read();
  MIDI2.read();
  MIDI3.read();

  PiperMidiAdapter<0>::loop();
  PiperMidiAdapter<1>::loop();
  PiperMidiAdapter<2>::loop();
  PiperMidiAdapter<3>::loop();
}