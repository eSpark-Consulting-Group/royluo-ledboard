#include "rgb.hpp"

ApplicationState status = OFF_STATE;

RgbAddressableLed rgb(PIN);


void setup() {

  Serial.begin(9600);

  pixels.begin();

  pinMode(BUTTON, INPUT);

  char royLuo[7] = "Roy Luo";

  char read[7];

  bool check = true;

  for (int i = 0; i < 7; i++) {
    read[i] = EEPROM.read(i + 3);

    if (read[i] != royLuo[i]) {
      check = false;
      break;
    }
  }

  if (!check) {
    for (int j = 0; j < 7; j++) {
      EEPROM.write(j + 3, royLuo[j]);
    }
    for (int i = 0; i < NO_OF_COLORS; i++) {
      EEPROM.write(i, i + 1);
    }
  }

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
