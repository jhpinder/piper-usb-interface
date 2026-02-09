#include "PiperUsbInterface.h"

PiperUsbInterface::PiperUsbInterface(Adafruit_USBD_MIDI& usbMidi) : usbMidi(usbMidi) {}

void PiperUsbInterface::begin() {
  if (!TinyUSBDevice.isInitialized()) {
    TinyUSBDevice.begin(0);
  }

  usbMidi.setStringDescriptor("Piper MIDI Interface");
}

void PiperUsbInterface::reMount() {
  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(250);
    TinyUSBDevice.attach();
  }
}

void PiperUsbInterface::loop() {
  TinyUSBDevice.task();

  if (!TinyUSBDevice.mounted()) {
    return;
  }
}