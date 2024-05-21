#define NO_OF_DELAYS 5
#define MAX_COLOR 17

const int red = 5;
const int green = 6;
const int blue = 7;
const int button = 3;

int counter = 0;
int buttonState = 0;
int reading;
int lastButtonState = 0;
unsigned long lastDebounceTime = millis();
unsigned long debounceTime = 50;
int rgbBrightness = 255;
bool programRunning = false;

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


struct LedPattern {
  unsigned long onDelay;
  unsigned long offDelay;
};

LedPattern cycleOfpattern[NO_OF_DELAYS] = {
  { 100, 100 },
  { 300, 300 },
  { 50, 100 },
  { 20, 200 },
  { 300, 0 },
};


struct rgbColor {
  int redColor;
  int greenColor;
  int blueColor;
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

void rgbWrite(rgbColor palette) {
  colorWrite(RED_PIN, palette.redColor);
  colorWrite(GREEN_PIN, palette.greenColor);
  colorWrite(BLUE_PIN, palette.blueColor);
}

void rgbPattern(bool status, LedPattern pattern, rgbColor palette) {
  if (status == true) {
    rgbWrite(palette.redColor);
    delay(pattern.onDelay);
    rgbWrite(palette[0]);
    delay(pattern.offDelay);
    rgbWrite(palette.blueColor);
    delay(pattern.onDelay);
    rgbWrite(BLUE, LOW);
    delay(pattern.offDelay);
    rgbWrite(GREEN, palette.greenColor);
    delay(pattern.onDelay);
    rgbWrite(GREEN, LOW);
    delay(pattern.offDelay);
  } else {
    rgbWrite(RED, LOW);
    rgbWrite(BLUE, LOW);
    rgbWrite(GREEN, LOW);
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
    }
  }
  if (programRunning) {

    if (reading != lastButtonState) {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceTime) {
      if (buttonState != reading) {
        reading = buttonState;
        if (reading == HIGH) {
          counter++;
          Serial.print("Initial counter is: ");
          Serial.println(counter);
        }
      } else {
        if (counter == 1) {
          Serial.print("Im in StandardStrobe: ");
          Serial.println(counter);
          rgbPattern(true, cycleOfpattern[0]);
        } else if (counter == 2) {
          Serial.print("Im in longStrobe: ");
          Serial.println(counter);
          rgbPattern(true, cycleOfpattern[1]);
        } else if (counter == 3) {
          Serial.print("Im in dots: ");
          Serial.println(counter);
          rgbPattern(true, cycleOfpattern[2]);
        } else if (counter == 4) {
          Serial.print("Im in flicker: ");
          Serial.println(counter);
          rgbPattern(true, cycleOfpattern[3]);
        } else if (counter == 5) {
          Serial.print("Im in ribbon: ");
          Serial.println(counter);
          rgbPattern(true, cycleOfpattern[4]);
        } else {
          counter = 1;
        }
      }
      lastButtonState = buttonState;
    }

  } else {
    for (int i = 0; i < MAX_COLOR; i++)
      // rgbWrite(i, LOW);
      delay(1000);
  }
}
