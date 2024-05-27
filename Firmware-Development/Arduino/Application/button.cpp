#include "button.hpp"

int Button::buttonState() {

  ButtonPress btnStatus = OFF;

  unsigned long debounceTime = 50;

  int btnState = digitalRead(button);

  if (btnState == HIGH) {

    unsigned long pressStartTime = millis();
    while (digitalRead(button) == HIGH && (millis() - pressStartTime) <= 2000) {
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