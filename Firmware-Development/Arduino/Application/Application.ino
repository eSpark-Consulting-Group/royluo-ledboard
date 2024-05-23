#define NO_OF_PATTERN 5
#define NO_OF_COLORS 3
#define MAX_COLOR 17

unsigned long lastDebounceTime = millis();
unsigned long debounceTime = 50;

const int red = 5;
const int green = 6;
const int blue = 7;
const int button = 3;

int reading;
int count = 0;
int counter = 0;
// int buttonState = 0;
int rgbBrightness = 255;
int lastButtonState = 0;

bool programRunning = false;

enum ButtonPress {
  SINGLE_PRESS,
  HOLD_PRESS,
  OFF
};

static ButtonPress btnStatus = OFF;

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

enum RGBType {
  RED = 1,
  BLUE,
  GREEN,
  YELLOW,
  LIME,
  ORANGE,
  WHITE,
  WARMWHITE,
  MAGENTA,
  LAVENDE,
  PURPLE,
  TURQUOISE,
  SKY_BLUE,
  CORAL,
  FOREST_GREEN,
  SILVER
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

  int btnState = digitalRead(button);

  if (btnState == HIGH) {

    unsigned long pressStartTime = millis();
    while (digitalRead(button) == HIGH && (millis() - pressStartTime) <= 2000) {
    }

    unsigned long T_Time = millis() - pressStartTime;
    if (T_Time >= 2000) {
      programRunning = !programRunning;
      if (programRunning) {
        btnStatus = HOLD_PRESS;
        count = 0;
      } else if(!programRunning){
        btnStatus = OFF;
      }
    } else {
      if (reading != lastButtonState) {
        lastDebounceTime = millis();
      }
      if ((millis() - lastDebounceTime) > debounceTime) {
        if (btnState != reading) {
          reading = btnState;
          if (reading == HIGH) {
            btnStatus = SINGLE_PRESS;
            programRunning = true;
          }
        }
      }
      lastButtonState = btnState;
    }

    if (btnStatus == SINGLE_PRESS) {
      state = LED_ON_STATE;
      status = ON_STATE;
      counter++;
    } else if (btnStatus == HOLD_PRESS) {
      state = LED_ON_STATE;
      status = COLOR_SELECTION_STATE;
      counter++;
    }
  } else if (btnStatus == OFF) {
    status = OFF_STATE;
    count = 0;
    counter = 0;
  }


  return btnStatus;
}

void rgbOffState() {
  if (buttonState() == OFF) {
    rgbWrite(colorPalette[0]);
    counter = 0;
  }
}

void rgbPatternState() {
  if (buttonState() == SINGLE_PRESS) {
    if (counter >= 1 && counter <= NO_OF_PATTERN) {
      Serial.print("Im in pattern: ");
      Serial.println(counter);
      rgbPattern(cycleOfpattern, cycleOfpattern.patterndelays[counter - 1]);
    }
    if (counter > NO_OF_PATTERN) {
      counter = 1;
    }
  }
}

void colorState() {
  if (buttonState() == HOLD_PRESS) {
    if (count == 0) {
      rgbWrite(colorPalette[3]);
      delay(2000);
      rgbWrite(colorPalette[1]);
      count = 1;
    }
    if (count == 1) {
      if (counter >= MAX_COLOR) {
        counter = 1;
      }
      Serial.print("Counter: ");
      Serial.println(counter);
      rgbWrite(colorPalette[counter]);
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {

  buttonState();

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