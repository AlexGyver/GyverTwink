void effects() {
  static Timer effTmr(30);
  static uint16_t countP = 0;
  static byte countSkip = 0;
  static byte prevEff = 255;
  static byte fadeCount = 0;

  if (effTmr.ready()) {
    byte thisEffect;

    if (forceTmr.state()) thisEffect = forceEff;
    else thisEffect = curEff;

    // эффект сменился
    if (prevEff != curEff) {
      prevEff = curEff;
      fadeCount = 25;
    }

    byte scale = effs[thisEffect].scale;
    byte speed = effs[thisEffect].speed;
    byte curPal = thisEffect;
    if (curPal >= ACTIVE_PALETTES) curPal -= ACTIVE_PALETTES;

    for (int i = 0; i < cfg.ledAm; i++) {
      byte idx;

      if (thisEffect < ACTIVE_PALETTES) {
        // первые ACTIVE_PALETTES эффектов - градиент
        // idx = map(xy[i][1], mm.minY, mm.maxY, 0, 255) + counter;   // прямой градиент
        idx = countP + ((mm.w * xy[i][0] / mm.h) + xy[i][1]) * scale / 100;   // диагональный градиент
      } else {
        // следующие - перлин нойс
        idx = inoise8(xy[i][0] * scale / 10, xy[i][1] * scale / 10, countP);
      }
      CRGB color = ColorFromPalette(paletteArr[curPal], idx, 255, LINEARBLEND);

      // плавная смена эффекта
      // меняется за 25 фреймов
      if (fadeCount) leds[i] = blend(leds[i], color, 40);
      else leds[i] = color;
    }
    if (fadeCount) fadeCount--;

    countP += (speed - 128) / 10;
    strip->showLeds(cfg.bright);
  }
}
