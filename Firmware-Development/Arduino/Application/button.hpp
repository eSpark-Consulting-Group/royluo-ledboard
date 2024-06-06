#include "Application.h"

enum ButtonState {
  SINGLE_PRESS,
  HOLD_PRESS,
  OFF
};

class Button {
  int pin;

public:
  Button(int pin) {
    this->pin = pin;
  }
  int getState();
};
