void handleButton() {
  static bool dir = 1;

  btn.tick();
  if (btn.hasClicks(1)) { // Один клик
    if (cfg.power) DEBUGLN("[Button] Disabling LEDs");
    else DEBUGLN("[Button] Enabling LEDs");
    cfg.power = !cfg.power;
    if (!cfg.power) strip->showLeds(0);
    EEcfg.update();
  }
  
  if (btn.hasClicks(2)) { // Два клика
    DEBUGLN("[Button] Next effect");
    switchEffect();
  }
  
  if (btn.step()) { // Удержание кнопки
    DEBUGLN("[Button] Changing brightnessness");
    if (cfg.power) {
      // меняем безопасно
      int bri = cfg.brightness;
      bri += dir ? 30 : -30;
      bri = constrain(bri, 0, 255);      
      cfg.brightness = bri;
      if (bri == 255) {
        strip->showLeds(0);
        delay(100);
        strip->showLeds(bri);
      }
      EEcfg.update();
    }
  }
  
  if (btn.releaseStep()) { // Отпустили кнопку после удержания
    DEBUGLN("[Button] Changing direction");
    dir = !dir;
  }
}
