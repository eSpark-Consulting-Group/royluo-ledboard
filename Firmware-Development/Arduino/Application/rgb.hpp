#include "button.hpp"

enum ColorType {
  RED_PIN,
  GREEN_PIN,
  BLUE_PIN
};

enum PatternState {
  LED_ON_STATE = 1,
  DELAY_START_STATE = 2,
  LED_OFF_STATE = 3,
  DELAY_END_STATE = 4
};


struct rgbColor {
  int redColor;
  int greenColor;
  int blueColor;
};

struct LedPatternDelay {
  unsigned long onDelay;
  unsigned long offDelay;
};

struct LedPatterns {
  rgbColor colors[NO_OF_COLORS + 1];
  LedPatternDelay patterndelays[NO_OF_PATTERN];
};


class RgbAddressableLed {
  int rgbPin;
public:
  RgbAddressableLed(int strip) {
    rgbPin = strip;
  }
  void eepromRead();

  void Write(rgbColor palette);

  void Pattern(LedPatterns pattern, LedPatternDelay patterndelay);

  void colorWrite(rgbColor color);

  void OffState();

  void PatternState();

  void colorState();
};
