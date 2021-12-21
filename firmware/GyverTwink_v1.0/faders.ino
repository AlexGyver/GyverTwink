void fader(CRGB color) {
  static uint32_t tmr;
  static int val = 0;
  static bool dir = true;
  if (millis() - tmr >= 20) {
    tmr = millis();
    val += dir ? 3 : -3;
    val = constrain(val, 5, 120);
    if (val >= 120 || val <= 5) dir = !dir;
    strip->showColor(color, val);
  }
}

void fadeBlink(CRGB color) {
  for (int i = 0; i < 200; i += 20) {
    strip->showColor(color, i);
    delay(20);
  }
  for (int i = 200; i > 0; i -= 20) {
    strip->showColor(color, i);
    delay(20);
  }
  strip->clearLedData();
  strip->showLeds(0);
}
