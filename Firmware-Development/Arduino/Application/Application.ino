#include <EEPROM.h>

#define NO_OF_PATTERN 5
#define NO_OF_COLORS 3
#define MAX_COLOR 17

const int red = 5;
const int green = 6;
const int blue = 7;
const int button = 3;


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

enum SetupStatus {
  SETUP_STATE,
  LOOP_STATE,
};

enum ButtonPress {
  SINGLE_PRESS,
  HOLD_PRESS,
  OFF
};

enum ApplicationState {
  ON_STATE,
  OFF_STATE,
  COLOR_SELECTION_STATE
};

static ApplicationState status = OFF_STATE;

enum PatternState {
  LED_ON_STATE = 1,
  DELAY_START_STATE = 2,
  LED_OFF_STATE = 3,
  DELAY_END_STATE = 4
};

static PatternState state = LED_ON_STATE;

enum ColorType {
  RED_PIN,
  GREEN_PIN,
  BLUE_PIN
};

struct LedPatternDelay {
  unsigned long onDelay;
  unsigned long offDelay;
};

struct rgbColor {
  int redColor;
  int greenColor;
  int blueColor;
};

struct LedPatterns {
  rgbColor colors[NO_OF_COLORS + 1];
  LedPatternDelay patterndelays[NO_OF_PATTERN];
};

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


void rgbWrite(rgbColor palette) {
  colorWrite(RED_PIN, palette.redColor);
  colorWrite(GREEN_PIN, palette.greenColor);
  colorWrite(BLUE_PIN, palette.blueColor);
}


void rgbPattern(LedPatterns pattern, LedPatternDelay patterndelay) {

  static unsigned long previousMillis;
  unsigned long currentMillis;

  static int patternStep = 1;
  currentMillis = millis();

  switch (state) {
    case LED_ON_STATE:
      rgbWrite(pattern.colors[patternStep]);
      previousMillis = currentMillis;
      state = DELAY_START_STATE;
      break;
    case DELAY_START_STATE:
      if (currentMillis - previousMillis >= patterndelay.onDelay) {
        state = LED_OFF_STATE;
      }
      break;
    case LED_OFF_STATE:
      rgbWrite(pattern.colors[0]);
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


void colorWrite(ColorType color, int brightness) {
  switch (color) {
    case RED_PIN:
      analogWrite(red, brightness);
      break;
    case GREEN_PIN:
      analogWrite(green, brightness);
      break;
    case BLUE_PIN:
      analogWrite(blue, brightness);
      break;
    default:
      break;
  }
}

int buttonState() {

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

void rgbOffState() {
  static SetupStatus functionState = SETUP_STATE;

  switch (functionState) {
    case SETUP_STATE:
      rgbWrite(colorPalette[0]);
      if (buttonState() == OFF) {
        functionState = LOOP_STATE;
      }
      break;

    case LOOP_STATE:
      ButtonPress btnStatus = buttonState();
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

void rgbPatternState() {

  static SetupStatus functionState = SETUP_STATE;
  static int counter = 1;

  switch (functionState) {
    case SETUP_STATE:
      rgbWrite(colorPalette[0]);
      if (buttonState() == OFF) {
        functionState = LOOP_STATE;
      }
      break;

    case LOOP_STATE:
      if (counter >= 1 && counter <= NO_OF_PATTERN) {
        Serial.print("Im in pattern: ");
        Serial.println(counter);
        rgbPattern(cycleOfpattern, cycleOfpattern.patterndelays[counter - 1]);
      }
      ButtonPress btnStatus = buttonState();
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

void colorState() {

  static SetupStatus functionState = SETUP_STATE;

  static int num = 0;
  static int counter = 0;

  switch (functionState) {
    case SETUP_STATE:
      if (counter == 0) {
        rgbWrite(colorPalette[3]);
        delay(2000);
        counter = 1;
        rgbWrite(colorPalette[counter]);
      }
      if (buttonState() == OFF) {
        functionState = LOOP_STATE;
      }
      break;

    case LOOP_STATE:
      ButtonPress btnStatus = buttonState();

      if (btnStatus == SINGLE_PRESS) {
        counter++;
        if (counter >= MAX_COLOR) {
          counter = 1;
        }
        Serial.print("Counter: ");
        Serial.println(counter);
        rgbWrite(colorPalette[counter]);

      } else if (btnStatus == HOLD_PRESS) {
        num++;
        if (num <= NO_OF_COLORS) {
          cycleOfpattern.colors[num] = colorPalette[counter];
          rgbWrite(colorPalette[0]);
          delay(500);
          rgbWrite(colorPalette[counter]);
          delay(500);
          rgbWrite(colorPalette[1]);
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

void setup() {

  int value = 0;

  Serial.begin(9600);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(button, INPUT);

  for (int i = 0; i < NO_OF_COLORS; i++) 
  {
    value = EEPROM.read(i);
    cycleOfpattern.colors[i+1] = colorPalette[value];
  }
}

void loop() {

  switch (status) {
    case ON_STATE:
      rgbPatternState();
      break;
    case OFF_STATE:
      rgbOffState();
      break;
    case COLOR_SELECTION_STATE:
      colorState();
      break;
    default:
      break;
  }
}