const int maxColor = 16;

const int red = 5;
const int green = 6;
const int blue = 7;
const int button = 3;

int buttonState = 0;
int counter = 0;
int pressStartTime = 0;

bool programRunning = false;
int rgbBrightness = 255;

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


void rgbWrite(RGB_Type pattern, int brightness) {
  switch (pattern) {
    case RED:
      colorWrite(RED_PIN, brightness);
      colorWrite(GREEN_PIN, LOW);
      colorWrite(BLUE_PIN, LOW);
      break;
    case BLUE:
      colorWrite(BLUE_PIN, brightness);
      colorWrite(RED_PIN, LOW);
      colorWrite(GREEN_PIN, LOW);
      break;
    case GREEN:
      colorWrite(RED_PIN, LOW);
      colorWrite(GREEN_PIN, brightness);
      colorWrite(BLUE_PIN, LOW);
      break;
    case YELLOW:
      colorWrite(RED_PIN, brightness);
      colorWrite(GREEN_PIN, brightness);
      colorWrite(BLUE_PIN, LOW);
      break;
    case LIME:
      if (brightness > 0) {
        colorWrite(RED_PIN, LOW);
        colorWrite(GREEN_PIN, 128);
        colorWrite(BLUE_PIN, LOW);
      }
      break;
    case ORANGE:
      if (brightness > 0) {
        colorWrite(RED_PIN, brightness);
        colorWrite(GREEN_PIN, 165);
        colorWrite(BLUE_PIN, LOW);
      }
      break;
    case WHITE:
      colorWrite(RED_PIN, brightness);
      colorWrite(GREEN_PIN, brightness);
      colorWrite(BLUE_PIN, brightness);
      break;
    case WARMWHITE:
      if (brightness > 0) {
        colorWrite(RED_PIN, 246);
        colorWrite(GREEN_PIN, 231);
        colorWrite(BLUE_PIN, 210);
      }
      break;
    case MAGENTA:
      colorWrite(RED_PIN, brightness);
      colorWrite(GREEN_PIN, LOW);
      colorWrite(BLUE_PIN, brightness);
      break;
    case LAVENDER:
      if (brightness > 0) {
        colorWrite(RED_PIN, 230);
        colorWrite(GREEN_PIN, 230);
        colorWrite(BLUE_PIN, 250);
      }
      break;
    case PURPLE:
      if (brightness > 0) {
        colorWrite(RED_PIN, 128);
        colorWrite(GREEN_PIN, LOW);
        colorWrite(BLUE_PIN, 128);
      }
      break;
    case TURQUOISE:
      if (brightness > 0) {
        colorWrite(RED_PIN, 64);
        colorWrite(GREEN_PIN, 224);
        colorWrite(BLUE_PIN, 208);
      }
      break;
    case SKY_BLUE:
      if (brightness > 0) {
        colorWrite(RED_PIN, 135);
        colorWrite(GREEN_PIN, 206);
        colorWrite(BLUE_PIN, 235);
      }
      break;
    case CORAL:
      if (brightness > 0) {
        colorWrite(RED_PIN, brightness);
        colorWrite(GREEN_PIN, 127);
        colorWrite(BLUE_PIN, 80);
      }
      break;
    case FOREST_GREEN:
      if (brightness > 0) {
        colorWrite(RED_PIN, 34);
        colorWrite(GREEN_PIN, 139);
        colorWrite(BLUE_PIN, 34);
      }
      break;
    case SILVER:
      if (brightness > 0) {
        colorWrite(RED_PIN, 192);
        colorWrite(GREEN_PIN, 192);
        colorWrite(BLUE_PIN, 192);
      }
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

    if (buttonState == HIGH) {
      delay(100);
      counter++;
      rgbWrite(counter, rgbBrightness);
      if (counter == maxColor) {
        counter = 0;
      }
    }
  } else {
    for (int i = 0; i < maxColor; i++)
      rgbWrite(i, LOW);
    delay(1000);
  }
}
