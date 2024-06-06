#include "Arduino.h"    
#include <EEPROM.h>

#include <tinyNeoPixel.h>

#define MAX_COLOR 17
#define NO_OF_PATTERN 5
#define NO_OF_COLORS 3

#define PIN 0
#define BUTTON 1

#define NUMPIXELS 1

extern tinyNeoPixel pixels;

enum SetupStatus {
  SETUP_STATE,
  LOOP_STATE,
};

enum ApplicationState {
  ON_STATE,
  OFF_STATE,
  COLOR_SELECTION_STATE
};
