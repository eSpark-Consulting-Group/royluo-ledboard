const int red = 5;
const int green = 6;
const int blue = 7;
const int button = 3;

int buttonState = 0;
int counter = 0;

bool programRunning = false;
int rgbBrightness = 255;


enum RGB_Type {
  RED = 1,
  GREEN = 2,
  BLUE = 3,
  YELLOW = 4,
  MAGENTA = 5,
  CYAN = 6,
  WHITE = 7,
  ORANGE = 8,
  VIOLET = 9,
  ROSE = 10

};


void pwm_Write(RGB_Type color, int brightness) {
  switch (color) {
    case RED:
      analogWrite(red, brightness);
      analogWrite(green, LOW);
      analogWrite(blue, LOW);
      break;
    case GREEN:
      analogWrite(red, LOW);
      analogWrite(green, brightness);
      analogWrite(blue, LOW);
      break;
    case BLUE:
      analogWrite(blue, brightness);
      analogWrite(red, LOW);
      analogWrite(green, LOW);
      break;
    case YELLOW:
      analogWrite(red, brightness);
      analogWrite(green, brightness);
      analogWrite(blue, LOW);
      break;
    case MAGENTA:
      analogWrite(red, brightness);
      analogWrite(green, LOW);
      analogWrite(blue, brightness);
      break;
    case CYAN:
      analogWrite(red, LOW);
      analogWrite(green, brightness);
      analogWrite(blue, brightness);
      break;
    case WHITE:
      analogWrite(red, brightness);
      analogWrite(green, brightness);
      analogWrite(blue, brightness);
      break;
    case ORANGE:
      if (brightness > 0) {
        analogWrite(red, brightness);
        analogWrite(green, 125);
        analogWrite(blue, LOW);
      } else {
        analogWrite(red, LOW);
        analogWrite(green, LOW);
        analogWrite(blue, LOW);
      }
      break;
    case VIOLET:
      if (brightness > 0) {
        analogWrite(red, 127);
        analogWrite(green, 0);
        analogWrite(blue, brightness);
      } else {
        analogWrite(red, LOW);
        analogWrite(green, LOW);
        analogWrite(blue, LOW);
      }
      break;
    case ROSE:
      if (brightness > 0) {
        analogWrite(red, brightness);
        analogWrite(green, 0);
        analogWrite(blue, 127);
      } else {
        analogWrite(red, LOW);
        analogWrite(green, LOW);
        analogWrite(blue, LOW);
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
      switch (counter) {
        case 1:
          pwm_Write(RED, rgbBrightness);
          break;
        case 2:
          pwm_Write(GREEN, rgbBrightness);
          break;
        case 3:
          pwm_Write(BLUE, rgbBrightness);
          break;
        case 4:
          pwm_Write(YELLOW, rgbBrightness);
          break;
        case 5:
          pwm_Write(MAGENTA, rgbBrightness);
          break;
        case 6:
          pwm_Write(CYAN, rgbBrightness);
          break;
        case 7:
          pwm_Write(WHITE, rgbBrightness);
          break;
        case 8:
          pwm_Write(ORANGE, rgbBrightness);
          break;
        case 9:
          pwm_Write(VIOLET, rgbBrightness);
          break;
        case 10:
          pwm_Write(ROSE, rgbBrightness);
          break;
        case 11:
          counter = 0;
          break;
      }
    }
  } else {
    pwm_Write(RED, LOW);
    pwm_Write(GREEN, LOW);
    pwm_Write(BLUE, LOW);
    pwm_Write(YELLOW, LOW);
    pwm_Write(MAGENTA, LOW);
    pwm_Write(CYAN, LOW);
    pwm_Write(WHITE, LOW);
    pwm_Write(ORANGE, LOW);
    pwm_Write(VIOLET, LOW);
    pwm_Write(ROSE, LOW);
    delay(1000);
  }
}