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
int buttonState = 0;
int rgbBrightness = 255;
int lastButtonState = 0;

bool programRunning = true;

enum State_type {
  LED_ON = 1,
  DELAY_START = 2,
  LED_OFF = 3,
  DELAY_END = 4
};

static State_type state = LED_ON;

enum Color_Type {
  RED_PIN = 1,
  GREEN_PIN = 2,
  BLUE_PIN = 3
};

enum RGB_Type {
  RED = 1,
  BLUE = 2,
  GREEN = 3,
  YELLOW = 4,
  LIME = 5,
  ORANGE = 6,
  WHITE = 7,
  WARMWHITE = 8,
  MAGENTA = 9,
  LAVENDER = 10,
  PURPLE = 11,
  TURQUOISE = 12,
  SKY_BLUE = 13,
  CORAL = 14,
  FOREST_GREEN = 15,
  SILVER = 16
};


struct LedPatternDelay {
  unsigned long onDelay;
  unsigned long offDelay;
};

struct rgbColor {
  int redColor;
  int blueColor;
  int greenColor;
};

struct LedPatterns {
  rgbColor colors[NO_OF_COLORS + 1];
  LedPatternDelay patterndelays[NO_OF_PATTERN];
};

rgbColor colorPalette[MAX_COLOR] = {
  { 0, 0, 0 },
  { 255, 0, 0 },
  { 0, 255, 0 },
  { 0, 0, 255 },
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
    case LED_ON:
      rgbWrite(pattern.colors[patternStep]);
      previousMillis = currentMillis;
      state = DELAY_START;
      break;
    case DELAY_START:
      if (currentMillis - previousMillis >= patterndelay.onDelay) {
        state = LED_OFF;
      }
      break;
    case LED_OFF:
      rgbWrite(pattern.colors[0]);
      previousMillis = currentMillis;
      state = DELAY_END;
      break;
    case DELAY_END:
      if (currentMillis - previousMillis >= patterndelay.offDelay) {
        state = LED_ON;
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


void colorWrite(Color_Type color, int brightness) {
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


void setup() {
  Serial.begin(9600);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {

  buttonState = digitalRead(button);

  if (buttonState == HIGH) {
    unsigned long pressStartTime = millis();
    while (digitalRead(button) == HIGH && (millis() - pressStartTime) <= 2000) {
    }

    unsigned long T_Time = millis() - pressStartTime;

    if (T_Time >= 2000) {
      programRunning = !programRunning;
      counter = 0;
      count++;
    }
  }
  if (count == 0) {
    if (programRunning) {
      if (reading != lastButtonState) {
        lastDebounceTime = millis();
      }
      if ((millis() - lastDebounceTime) > debounceTime) {
        if (buttonState != reading) {
          reading = buttonState;
          if (reading == HIGH) {
            counter++;
            state = LED_ON;
            Serial.print("Initial counter is: ");
            Serial.println(counter);
          }
        } else {
          if (counter >= 1 && counter <= 5) {
            Serial.print("Im in pattern: ");
            Serial.println(counter);
            rgbPattern(cycleOfpattern, cycleOfpattern.patterndelays[counter - 1]);
          }
          if (counter > 5) {
            counter = 1;
          }
        }
        lastButtonState = buttonState;
      }
    }
  } 
  ////////////////////////////////////////
  if (count == 2) {
    Serial.print("Im in count: ");
    Serial.println(count);
    if (programRunning) {
      rgbWrite(colorPalette[3]);
      delay(2000);
      rgbWrite(colorPalette[1]);
      programRunning = !programRunning;
      counter = 0;
    }
    if (!programRunning) {
      if (buttonState == HIGH) {
        counter++;
        if (counter >= 1 && counter <= MAX_COLOR) {
          rgbWrite(colorPalette[counter]);
        } else {
          counter = 0;
        }
      }
    }
  } 
  ///////////////////////////////////////////
  if(count != 0 && count != 2)
  {
    rgbWrite(colorPalette[0]);
    if(count == 3)
    {
      buttonState = digitalRead(button);
      if(buttonState == HIGH)
      {
         count = 0;
      }
    }
  }
}