#include "Arduino.h"    
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define MAX_COLOR 17
#define NO_OF_PATTERN 5
#define NO_OF_COLORS 3

#define LED 0
#define PIN 6
#define BUTTON 3

#define NUMPIXELS 1

extern Adafruit_NeoPixel pixels;

enum SetupStatus {
  SETUP_STATE,
  LOOP_STATE,
};

enum ApplicationState {
  ON_STATE,
  OFF_STATE,
  COLOR_SELECTION_STATE
};




