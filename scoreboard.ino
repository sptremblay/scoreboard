#include <FastLED.h>
#include <EEPROM.h>

//Pins
#define BUTTON_START_STOP_PIN 11
#define CHRONO_DATA_PIN 6
#define SCORE_HOME_DATA_PIN 7
#define SCORE_AWAY_DATA_PIN 8

#define BUTTON_HOME_SCORE_UP_PIN 9
#define CLOCK_PIN 13

// constant
#define LED_SEGMENTS 3 //number leds per segment
#define SEGMENT 7 //number of segment
#define NUM_LEDS_DIGIT LED_SEGMENTS*SEGMENT //number leds per digit
#define BRIGHTNESS  60

int lastButtonState;
int start_stop_buttonState;
int start_stop_lastButtonState;

unsigned long start_stop_button_debounce_time = 0;

unsigned int debounce_delay = 50;

int chrono_started;

const int charLayout[10][7] = {
  {0, 1, 1, 1, 1, 1, 1}, //0
  {0, 0, 0, 1, 1, 0, 0}, //1
  {1, 1, 1, 0, 1, 1, 0}, //2
  {1, 0, 1, 1, 1, 1, 0}, //3
  {1, 0, 0, 1, 1, 0, 1}, //4
  {1, 0, 1, 1, 0, 1, 1}, //5
  {1, 1, 1, 1, 0, 1, 1}, //6
  {0, 0, 0, 1, 1, 1, 0}, //7
  {1, 1, 1, 1, 1, 1, 1}, //8
  {1, 0, 1, 1, 1, 1, 1} //9
};

void setup() {
  Serial.begin(9600);
  FastLED.setBrightness(  BRIGHTNESS );
  Serial.println("setup");

  // Uncomment/edit one of the following lines for your leds arrangement.
  setupChrono();
  //setupScores();

  pinMode(BUTTON_START_STOP_PIN, INPUT);
  pinMode(BUTTON_HOME_SCORE_UP_PIN, INPUT);

  chrono_started = false;
}

void loop() {
  SetChronoTime();
  switch (ReadButton()) {
    case 1:
      chrono_started = true;
      break;
    case 2:
      chrono_started = false;
      break;
    case 3:
      incrementScore(0, 1);
      break;
    case 4:
      incrementScore(1, 1);
      break;
  }

}

int ReadButton() {
  int button_selected = 0;
  int reading1 = digitalRead(BUTTON_START_STOP_PIN);
  int score_home_up_buttonState = digitalRead(BUTTON_HOME_SCORE_UP_PIN);

  //Serial.println(reading1);

  if (start_stop_buttonState != start_stop_lastButtonState) {
    start_stop_button_debounce_time = millis();
  }

  if ((millis() - start_stop_button_debounce_time) > debounce_delay) {
    start_stop_buttonState = reading1;
    // If the button state has changed to pressed.
    if (start_stop_buttonState != start_stop_lastButtonState &&
        start_stop_buttonState == HIGH && chrono_started == false) {
      button_selected = 1;
      Serial.println("start");
    }

    if (start_stop_buttonState != start_stop_lastButtonState &&
        start_stop_buttonState == HIGH && chrono_started == true) {
      button_selected = 2;
      Serial.println("stop");
    }
  }
  /*if (start_stop_buttonState == 1 && lastButtonState == 0 &&  chrono_started == false) {
    button_selected = 1;
    lastButtonState = 1;
    Serial.println("Start");
    delay(5);
    } else if (start_stop_buttonState == 1 && lastButtonState == 0  &&  chrono_started == true) {
    lastButtonState = 1;
    button_selected = 2;
    Serial.println("Stop");
    delay(5);
    } else if (score_home_up_buttonState == 1 && lastButtonState == 0 ) {
    lastButtonState = 1;
    button_selected = 3;
    delay(5);
    Serial.print("Score Home UP");
    }
    else {
    lastButtonState = start_stop_buttonState || score_home_up_buttonState;
    }
  */
  start_stop_lastButtonState = reading1;
  return button_selected;
}

// Display functions ....
void setNumber(CRGB* leds, int number, int offset) {
  int ones = (number % 10);
  int tens = ((number / 10) % 10);
  for (int k = 0; k < SEGMENT; k++) {
    setDigit(leds, k, charLayout[ones][k], offset);
  }

  for (int k = 0; k < SEGMENT; k++) {
    setDigit(leds, k, charLayout[tens][k], offset + NUM_LEDS_DIGIT);
  }
}

void setDigit(CRGB* leds, int segment, int ledState, int offset) {
  int start_loc = LED_SEGMENTS * segment + offset;
  if (ledState == 0) {
    for (int loc = start_loc; loc < start_loc + LED_SEGMENTS; loc++) {
      leds[loc] = CRGB::Black;
    }
  } else {
    for (int loc = start_loc; loc < start_loc + LED_SEGMENTS; loc++) {
      leds[loc] = CRGB::White;
    }
  }
}
