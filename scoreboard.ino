#include <FastLED.h>
#include <EEPROM.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


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

RF24 radio(22, 23); // CE, CSN
const byte address[6] = "00001";

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

  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_LOW);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver

  chrono_started = true;
}

bool first = true;
void loop() {

  SetChronoTime();
  displayScores();
  displayPeriod();

  
  /*int commandId =  0;
  if (radio.available())              //Looking for the data.
  {
    radio.read(&commandId, sizeof(commandId));    //Reading the data
    if (commandId>0) {
      Serial.println("command: " + commandId);
    }
  }*/

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
