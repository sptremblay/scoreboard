#include <FastLED.h>
#include <EEPROM.h>

//Pins
#define BUTTON_START_STOP_PIN 12
#define CHRONO_DATA_PIN 7
#define SCORE_HOME_DATA_PIN 5
#define SCORE_AWAY_DATA_PIN 8
#define PERIOD_DATA_PIN 9

#define HORN_RELAY_PIN 11

#define BUTTON_HOME_SCORE_UP_PIN 9
#define CLOCK_PIN 13

// constant
#define LED_SEGMENTS 3 //number leds per segment
#define SEGMENT 7 //number of segment
#define NUM_LEDS_DIGIT LED_SEGMENTS*SEGMENT //number leds per digit
#define BRIGHTNESS  128

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
  setupScores();
  setupPeriod();

  pinMode(HORN_RELAY_PIN, OUTPUT);

  pinMode(BUTTON_START_STOP_PIN, INPUT);
  pinMode(BUTTON_HOME_SCORE_UP_PIN, INPUT);

  chrono_started = true;
}

bool first = true;
void loop() {

  /*if(first == true){
    digitalWrite(HORN_RELAY_PIN, LOW);
    delay(200);
    digitalWrite(HORN_RELAY_PIN, HIGH);
    first = false;
  }*/
  
  SetChronoTime();
  displayScores();
  displayPeriod();
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
  start_stop_lastButtonState = reading1;
  return button_selected;
}

// Display functions ....
void setNumber(CRGB* leds, int number, int offset) {
  int ones = (number % 10);
  int tens = ((number / 10) % 10);
  for (int k = 0; k < SEGMENT; k++) {
    setDigit(leds, k, charLayout[ones][k], offset, LED_SEGMENTS);
  }

  for (int k = 0; k < SEGMENT; k++) {
    setDigit(leds, k, charLayout[tens][k], offset + NUM_LEDS_DIGIT, LED_SEGMENTS);
  }
}

void setSmallNumber(CRGB* leds, int number, int offset) {
  int ones = (number % 10);
  for (int k = 0; k < SEGMENT; k++) {
    setDigit(leds, k, charLayout[ones][k], offset, 2);
  }
}

void setDigit(CRGB* leds, int segment, int ledState, int offset, int numberLeds) {
  int start_loc = numberLeds * segment + offset;
  if (ledState == 0) {
    for (int loc = start_loc; loc < start_loc + numberLeds; loc++) {
      leds[loc] = CRGB::Black;
    }
  } else {
    for (int loc = start_loc; loc < start_loc + numberLeds; loc++) {
      leds[loc] = CRGB::White;
    }
  }
}
