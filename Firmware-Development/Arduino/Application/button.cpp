#include "button.hpp"

int Button::getState() {

  ButtonState btnStatus = OFF;

  unsigned long debounceTime = 50;

  int btnState = digitalRead(pin);

  if (btnState == LOW) {

    unsigned long pressStartTime = millis();
    while (digitalRead(pin) == LOW && (millis() - pressStartTime) <= 2000) {
    }

    unsigned long T_Time = millis() - pressStartTime;
    if (T_Time >= 2000) {
      btnStatus = HOLD_PRESS;
    } else {
      static unsigned long lastDebounceTime = 0;

      if ((millis() - lastDebounceTime) > debounceTime) {
        lastDebounceTime = millis();
        btnStatus = SINGLE_PRESS;
      }
    }
  }

  return btnStatus;
}
