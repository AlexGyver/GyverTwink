/***
 * Обработчик кнопки
 */
void handleButton() {
  static bool dir = 1;

  btn.tick();
  if (btn.hasClicks(1)) {
    DEBUGLN("[Button] Power on/off LEDs");
    cfg.power = !cfg.power;
    if (!cfg.power) strip->showLeds(0);
    EEcfg.update();
  }
  
  if (btn.hasClicks(2)) {
    DEBUGLN("[Button] Switching effect");
    switchEffect();
  }
  
  if (btn.step()) {
    DEBUGLN("[Button] Change brightness");
    if (cfg.power) {
      // меняем безопасно
      int bri = cfg.bright;
      bri += dir ? 30 : -30;
      bri = constrain(bri, 0, 255);      
      cfg.bright = bri;
      if (bri == 255) {
        strip->showLeds(0);
        delay(100);
        strip->showLeds(bri);
      }
      EEcfg.update();
    }
  }
  
  if (btn.releaseStep()) {
    DEBUGLN("[Button] Changing direction");
    dir = !dir;
  }
}
