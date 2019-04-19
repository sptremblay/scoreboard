
#define SCORE_LEDS NUM_LEDS_DIGIT//*2

CRGB score_home_leds[21];
CRGB score_away_leds[21];

int score_home = 0;
int score_away = 0;

/*void setupScores() {
  FastLED.addLeds<WS2811, SCORE_HOME_DATA_PIN, RGB>(score_home_leds, SCORE_LEDS);
  FastLED.addLeds<WS2811, SCORE_AWAY_DATA_PIN, RGB>(score_away_leds, SCORE_LEDS);
  displayScores();
}*/

void incrementScore(int team, int score) {
  if (team == 0) {
    score_home = score_home + score;
    Serial.println(score_home);
  } else {
    score_away = score_away + score;
  }
  //Serial.print("Score Home");
  //Serial.println(score_home);
  displayScores();
}

void displayScores() {
  setNumber(score_home_leds, score_home,0);
  setNumber(score_away_leds, score_away,0);
  FastLED.show();
}
