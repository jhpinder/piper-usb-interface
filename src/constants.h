#include "piper-midi.h"
#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <MIDI.h>

#define MIDI_RCV_BUFFER_SIZE 512

#define RS485_BAUD_RATE 1000000
#define RS485_TX_PIN 0