#include "rgb.hpp"

extern ApplicationState status;

static PatternState state = LED_ON_STATE;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Button button(BUTTON);

rgbColor colorPalette[MAX_COLOR] = {
  { 0, 0, 0 },
  { 255, 0, 0 },
  { 0, 0, 255 },
  { 0, 255, 0 },
  { 255, 255, 0 },
  { 0, 128, 0 },
  { 255, 165, 0 },
  { 255, 255, 255 },
  { 246, 231, 210 },
  { 255, 0, 255 },
  { 230, 230, 250 },
  { 128, 0, 128 },
  { 64, 224, 208 },
  { 135, 206, 235 },
  { 255, 127, 80 },
  { 34, 139, 34 },
  { 192, 192, 192 },
};

LedPatterns cycleOfpattern = {
  .colors = { colorPalette[0], colorPalette[1], colorPalette[2], colorPalette[3] },
  .patterndelays = {
    { 150, 150 },
    { 350, 350 },
    { 50, 150 },
    { 20, 250 },
    { 400, 0 },
  }
};

void RgbAddressableLed::Write(rgbColor palette) {
  colorWrite(palette);
}

void RgbAddressableLed::Pattern(LedPatterns pattern, LedPatternDelay patterndelay) {

  static unsigned long previousMillis;
  unsigned long currentMillis;

  static int patternStep = 1;
  currentMillis = millis();

  switch (state) {
    case LED_ON_STATE:
      Write(pattern.colors[patternStep]);
      previousMillis = currentMillis;
      state = DELAY_START_STATE;
      break;
    case DELAY_START_STATE:
      if (currentMillis - previousMillis >= patterndelay.onDelay) {
        state = LED_OFF_STATE;
      }
      break;
    case LED_OFF_STATE:
      Write(pattern.colors[0]);
      previousMillis = currentMillis;
      state = DELAY_END_STATE;
      break;
    case DELAY_END_STATE:
      if (currentMillis - previousMillis >= patterndelay.offDelay) {
        state = LED_ON_STATE;
        patternStep++;
        if (patternStep > NO_OF_COLORS) {
          patternStep = 1;
        }
      }
      break;
    default:
      break;
  }
}

void RgbAddressableLed::colorWrite(rgbColor palette) {
      pixels.clear();
      for(int i = 0; i <= NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(palette.redColor,palette.greenColor,palette.blueColor));
      }
      pixels.show();
}

void RgbAddressableLed::OffState() {
  static SetupStatus functionState = SETUP_STATE;

  switch (functionState) {
    case SETUP_STATE:
      Write(colorPalette[0]);
      if (button.buttonState() == OFF) {
        functionState = LOOP_STATE;
      }
      break;

    case LOOP_STATE:
      ButtonPress btnStatus = button.buttonState();
      if (btnStatus == SINGLE_PRESS) {
        status = ON_STATE;
        functionState = SETUP_STATE;
      } else if (btnStatus == HOLD_PRESS) {
        status = COLOR_SELECTION_STATE;
        functionState = SETUP_STATE;
      }
      break;
  }
}

void RgbAddressableLed::PatternState() {

  static SetupStatus functionState = SETUP_STATE;
  static int counter = 1;

  switch (functionState) {
    case SETUP_STATE:
      Write(colorPalette[0]);
      if (button.buttonState() == OFF) {
        functionState = LOOP_STATE;
      }
      break;

    case LOOP_STATE:
      if (counter >= 1 && counter <= NO_OF_PATTERN) {
        Serial.print("Im in pattern: ");
        Serial.println(counter);
        Pattern(cycleOfpattern, cycleOfpattern.patterndelays[counter - 1]);
      }
      ButtonPress btnStatus = button.buttonState();
      if (btnStatus == SINGLE_PRESS) {
        counter++;
        if (counter > NO_OF_PATTERN) {
          counter = 1;
        }
      } else if (btnStatus == HOLD_PRESS) {
        status = OFF_STATE;
        functionState = SETUP_STATE;
        counter = 1;
        Serial.println("in off state");
      }
      break;
  }
}

void RgbAddressableLed::colorState() {

  static SetupStatus functionState = SETUP_STATE;

  static int num = 0;
  static int counter = 0;

  switch (functionState) {
    case SETUP_STATE:
      if (counter == 0) {
        Write(colorPalette[3]);
        delay(2000);
        counter = 1;
        Write(colorPalette[counter]);
      }
      if (button.buttonState() == OFF) {
        functionState = LOOP_STATE;
      }
      break;

    case LOOP_STATE:
      ButtonPress btnStatus = button.buttonState();

      if (btnStatus == SINGLE_PRESS) {
        counter++;
        if (counter >= MAX_COLOR) {
          counter = 1;
        }
        Serial.print("Counter: ");
        Serial.println(counter);
        Write(colorPalette[counter]);

      } else if (btnStatus == HOLD_PRESS) {
        num++;
        if (num <= NO_OF_COLORS) {
          cycleOfpattern.colors[num] = colorPalette[counter];
          Write(colorPalette[0]);
          delay(500);
          Write(colorPalette[counter]);
          delay(500);
          Write(colorPalette[1]);
          EEPROM.write(num - 1, counter);
          counter = 1;
          Serial.print(num);
          Serial.println(" color selected");
          functionState = SETUP_STATE;
          if (num == NO_OF_COLORS) {
            num = 0;
            counter = 0;
            status = ON_STATE;
          }
        }
      }
      break;
  }
}

void RgbAddressableLed::eepromRead() {

  Serial.println("Im in EEPROM Function");

  int value = 0;

  for (int i = 0; i < NO_OF_COLORS; i++) {
    value = EEPROM.read(i);
    Serial.print(" EEPROM Value is: ");
    Serial.println(value);
    if (value > 0 && value <= 16) {
      cycleOfpattern.colors[i + 1] = colorPalette[value];
      Serial.println(" RGB blink: ");
    } else {
      cycleOfpattern.colors[i + 1] = colorPalette[i + 1];
    }
  }
}
