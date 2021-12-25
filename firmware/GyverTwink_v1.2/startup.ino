void setupNetwork() {
  portalStart(); // Стартуем SimplePortal
  DEBUGLN("[Setup] SimplePortal started!");

  // Ждём действий пользователя, мигаем
  while (!portalTick()) fader(CRGB::Blue);

  // 1 connect, 2 ap, 3 local, обновляем данные в епр
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

bool buttonState() {
  uint32_t Timer = millis();
  while (millis() - Timer < 2000) {
    btn.tick();
    if (btn.state()) return true;
  }
  return false;
}

void accessPoint() {
  DEBUG("AP Mode");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(GT_AP_SSID, GT_AP_PASS, GT_AP_CHANNEL, GT_AP_HIDDEN, GT_AP_MAX);
  currentIP = WiFi.softAPIP();
  server.begin();
  fadeBlink(CRGB::Magenta);
}

void routerConnect() {
  DEBUG("Connecting to AP... ");
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(portalCfg.SSID, portalCfg.pass);
  uint32_t Timer = millis();
  bool state = false;
  while (millis() - Timer < 15000) {
    if (WiFi.status() == WL_CONNECTED) {
      fadeBlink(CRGB::Green);
      DEBUGLN("ok");
      currentIP = WiFi.localIP();
      return;
    }
    fader(CRGB::Yellow);
    yield();
  }
  fadeBlink(CRGB::Red);
  DEBUGLN("fail");
  accessPoint();
}
