
#define PERIOD_LEDS 14 

CRGB period_leds[PERIOD_LEDS];

int period = 1;

void setupPeriod() {
  FastLED.addLeds<WS2811, PERIOD_DATA_PIN, RGB>(period_leds, PERIOD_LEDS);
  displayPeriod();
}


void displayPeriod() {
  setSmallNumber(period_leds, period,0);
  /*for(int dot = 0; dot < PERIOD_LEDS; dot++) { 
      period_leds[dot] = CRGB::White;
      FastLED.show();
  }*/
  FastLED[3].showLeds(BRIGHTNESS);
}
