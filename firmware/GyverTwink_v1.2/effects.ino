void playEffects() {
  static Timer effectTimer(30);
  static uint16_t offset = 0;
  static byte countSkip = 0;
  static byte previousEffect = 255;
  static byte fadeCount = 0;

  // Если таймер отрисовки эффектов сработал
  if (effectTimer.ready()) {
    byte thisEffect;

    // Принудительно меняем эффект
    if (forceTimer.state()) thisEffect = forcedEffect;
    else thisEffect = currentEffect;

    // Эффект сменился
    if (previousEffect != currentEffect) {
      previousEffect = currentEffect;
      fadeCount = 25;
    }

    // Данные об эффекте
    byte scale = effects[thisEffect].scale;
    byte speed = effects[thisEffect].speed;
    byte currentPallete = thisEffect;
    if (currentPallete >= ACTIVE_PALETTES) currentPallete -= ACTIVE_PALETTES;

    // Для каждой лампочки
    for (int i = 0; i < cfg.ledAmount; i++) {
      byte idx;

      if (thisEffect < ACTIVE_PALETTES) {
        // Первые ACTIVE_PALETTES эффектов - градиент
        // idx = map(xy[i][1], mm.minY, mm.maxY, 0, 255) + counter;           // прямой градиент
        idx = offset + ((mm.w * xy[i][0] / mm.h) + xy[i][1]) * scale / 100;   // диагональный градиент
      } else {
        // Следующие - Perlin Noise
        idx = inoise8(xy[i][0] * scale / 10, xy[i][1] * scale / 10, offset);
      }
      // Получаем цвет
      CRGB color = ColorFromPalette(paletteArr[currentPallete], idx, 255, LINEARBLEND);

      // Плавная смена эффекта, меняется за 25 повторений
      if (fadeCount) leds[i] = blend(leds[i], color, 40);
      else leds[i] = color;
    }
    if (fadeCount) fadeCount--;

    offset += (speed - 128) / 10;
    strip->showLeds(cfg.brightness);
  }
}
