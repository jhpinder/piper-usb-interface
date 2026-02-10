#include "PiperMidiAdapter.h"
#include "PiperUsbInterface.h"

Adafruit_USBD_MIDI usbMidi(4);
PiperUsbInterface piperUsbInterface(usbMidi);

midi::SerialMIDI<Adafruit_USBD_MIDI> serialMIDI0(usbMidi);
midi::SerialMIDI<Adafruit_USBD_MIDI> serialMIDI1(usbMidi);
midi::SerialMIDI<Adafruit_USBD_MIDI> serialMIDI2(usbMidi);
midi::SerialMIDI<Adafruit_USBD_MIDI> serialMIDI3(usbMidi);

void setup() {
  // MIDI_RCV_BUFFER_SIZE must be a power of 2
  static_assert((MIDI_RCV_BUFFER_SIZE & (MIDI_RCV_BUFFER_SIZE - 1)) == 0);

  piperUsbInterface.begin();
  piperUsbInterface.reMount();

  PiperMidiAdapter<RS485_TX_PIN0>::begin();
  PiperMidiAdapter<RS485_TX_PIN1>::begin();
  PiperMidiAdapter<RS485_TX_PIN2>::begin();
  PiperMidiAdapter<RS485_TX_PIN3>::begin();
}

void loop() {
  piperUsbInterface.loop();

  uint8_t packet[4];
  while (usbMidi.readPacket(packet)) {
    uint8_t cable = packet[0] >> 4;
    uint8_t status = packet[1] & 0xF0;
    uint8_t channel = packet[1] & 0x0F;
    uint8_t note = packet[2];
    uint8_t velocity = packet[3];

    bool isNoteOn = (status == 0x90 && velocity > 0);
    bool isNoteOff = (status == 0x80 || (status == 0x90 && velocity == 0));

    if (!isNoteOn && !isNoteOff)
      continue;

    switch (cable) {
    case 0:
      if (isNoteOn)
        PiperMidiAdapter<RS485_TX_PIN0>::handleNoteOn(channel, note, velocity);
      else
        PiperMidiAdapter<RS485_TX_PIN0>::handleNoteOff(channel, note, velocity);
      break;
    case 1:
      if (isNoteOn)
        PiperMidiAdapter<RS485_TX_PIN1>::handleNoteOn(channel, note, velocity);
      else
        PiperMidiAdapter<RS485_TX_PIN1>::handleNoteOff(channel, note, velocity);
      break;
    case 2:
      if (isNoteOn)
        PiperMidiAdapter<RS485_TX_PIN2>::handleNoteOn(channel, note, velocity);
      else
        PiperMidiAdapter<RS485_TX_PIN2>::handleNoteOff(channel, note, velocity);
      break;
    case 3:
      if (isNoteOn)
        PiperMidiAdapter<RS485_TX_PIN3>::handleNoteOn(channel, note, velocity);
      else
        PiperMidiAdapter<RS485_TX_PIN3>::handleNoteOff(channel, note, velocity);
      break;
    }
  }

  PiperMidiAdapter<RS485_TX_PIN0>::loop();
  PiperMidiAdapter<RS485_TX_PIN1>::loop();
  PiperMidiAdapter<RS485_TX_PIN2>::loop();
  PiperMidiAdapter<RS485_TX_PIN3>::loop();
}