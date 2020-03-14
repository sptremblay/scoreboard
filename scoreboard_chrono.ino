#define NUMBER_LEDS_CHRONO NUM_LEDS_DIGIT *4+2

// Define the array of leds
CRGB chrono_leds[NUMBER_LEDS_CHRONO];

int chrono_time = 1200; // time in seconds
unsigned long previousMillis = 0;
const long interval = 1000; // every second

void setupChrono() {
  FastLED.addLeds<WS2811, CHRONO_DATA_PIN, RGB>(chrono_leds, NUMBER_LEDS_CHRONO);
  showScoreboardTime();

  chrono_leds[42] = CRGB::Red;
  chrono_leds[43] =  CRGB::Red;
  Serial.println("Setup Chrono");

  FastLED[0].showLeds();
}

void SetChronoTime() {
  unsigned long currentMillis = millis();
  if (chrono_started == true) {
    if (currentMillis - previousMillis >= interval) {
      // make sure time is set every seconds
      previousMillis = currentMillis;
      chrono_time = chrono_time - 1;
      if (chrono_time == 0) {
        chrono_started = false;
        //trigger end
      }
      showScoreboardTime();
    }
  }
}

void showScoreboardTime() {
  int minutes = chrono_time / 60;
  int secondes = chrono_time % 60;
  Serial.print( minutes);
  Serial.print(":");
  Serial.println(secondes);
  setNumber(chrono_leds, secondes,0);
  setNumber(chrono_leds, minutes, 44);//minutes,NUM_LEDS_DIGIT*2+2);
  FastLED[0].showLeds(BRIGHTNESS);
}
