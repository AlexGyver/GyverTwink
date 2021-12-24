void portalRoutine() {
  // запускаем portal
  portalStart();
  DEBUGLN("Portal start");

  // ждём действий пользователя, мигаем
  while (!portalTick()) fader(CRGB::Blue);

  // если это 1 connect, 2 ap, 3 local, обновляем данные в епр
  if (portalStatus() <= 3) EEwifi.updateNow();

  DEBUG("Portal status: ");
  DEBUGLN(portalStatus());
}

void startStrip() {
  strip = &FastLED.addLeds<LED_TYPE, LED_PIN, LED_ORDER>(leds, LED_MAX).setCorrection(TypicalLEDStrip);
  strip->setLeds(leds, LED_MAX);
  strip->clearLedData();
  // выводим ргб
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Blue;
  strip->showLeds(50);
}

bool checkButton() {
  uint32_t tmr = millis();
  while (millis() - tmr < 2000) {
    btn.tick();
    if (btn.state()) return true;
  }
  return false;
}

void setupAP() {
  DEBUG("AP Mode");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(GT_AP_SSID, GT_AP_PASS);
  myIP = WiFi.softAPIP();
  server.begin();
  fadeBlink(CRGB::Magenta);
}

void setupSTA() {
  DEBUG("Connecting to AP... ");
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(portalCfg.SSID, portalCfg.pass);
  uint32_t tmr = millis();
  bool state = false;
  while (millis() - tmr < 15000) {
    if (WiFi.status() == WL_CONNECTED) {
      fadeBlink(CRGB::Green);
      DEBUGLN("ok");
      myIP = WiFi.localIP();
      return;
    }
    fader(CRGB::Yellow);
    yield();
  }
  fadeBlink(CRGB::Red);
  DEBUGLN("fail");
  setupAP();
}
