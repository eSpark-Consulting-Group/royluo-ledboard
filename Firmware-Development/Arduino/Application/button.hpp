#include "Application.h"

enum ButtonPress {
  SINGLE_PRESS,
  HOLD_PRESS,
  OFF
};

class Button {
  int button;

public:
  Button(int btn) {
    button = btn;
  }
  int buttonState();
};
