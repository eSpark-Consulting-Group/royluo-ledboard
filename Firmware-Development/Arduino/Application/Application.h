#include "Arduino.h"             
#include <EEPROM.h>

#define MAX_COLOR 17
#define NO_OF_PATTERN 5
#define NO_OF_COLORS 3

#define RED 5
#define GREEN 6
#define BLUE 7
#define BUTTON 3

enum SetupStatus {
  SETUP_STATE,
  LOOP_STATE,
};

enum ApplicationState {
  ON_STATE,
  OFF_STATE,
  COLOR_SELECTION_STATE
};



