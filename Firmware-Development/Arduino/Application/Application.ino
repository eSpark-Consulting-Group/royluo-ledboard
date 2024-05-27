#include "rgb.hpp"

ApplicationState status = OFF_STATE;

RgbAddressableLed rgb(RED, GREEN, BLUE);

void setup() {

  Serial.begin(9600);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(BUTTON, INPUT);

  rgb.eepromRead();
}

void loop() {

  switch (status) {
    case ON_STATE:
      rgb.PatternState();
      break;
    case OFF_STATE:
      rgb.OffState();
      break;
    case COLOR_SELECTION_STATE:
      rgb.colorState();
      break;
    default:
      break;
  }
}





// enum RGBType {
//   RED = 1,
//   BLUE,
//   GREEN,
//   YELLOW,
//   LIME,
//   ORANGE,
//   WHITE,
//   WARMWHITE,
//   MAGENTA,
//   LAVENDE,
//   PURPLE,
//   TURQUOISE,
//   SKY_BLUE,
//   CORAL,
//   FOREST_GREEN,
//   SILVER
// };