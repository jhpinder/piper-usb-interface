#include "piper-midi.h"
#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <MIDI.h>

#define MIDI_RCV_BUFFER_SIZE 1024
#define PIPER_BATCH_SIZE 32
#define RS485_BAUD_RATE 1000000
#define RS485_TX_PIN0 0
#define RS485_TX_PIN1 1
#define RS485_TX_PIN2 2
#define RS485_TX_PIN3 3