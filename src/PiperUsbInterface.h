#include "constants.h"

class PiperUsbInterface {
public:
  PiperUsbInterface(Adafruit_USBD_MIDI& usbMidi);
  void begin();
  void reMount();
  void loop();

  Adafruit_USBD_MIDI& usbMidi;
};