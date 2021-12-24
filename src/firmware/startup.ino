/***
 * Запустить SimplePortal
 */
void configMode() {
  // Запускаем SimplePortal
  portalStart();
  DEBUGLN("[SimplePortal] Portal started");

  // Ждём действий пользователя, мигаем
  while (!portalTick()) fader(CRGB::Blue);

  // Режимы: 1 - Router, 2 - Access Point, 3 - Local
  // Если статус один из них, меняем режим
  if (portalStatus() <= 3) EEwifi.updateNow();

  DEBUG("[SimplePortal] Portal status: ");
  DEBUGLN(portalStatus());
}

/***
 * Запустить ленту
 */
void startLEDs() {
  strip = &FastLED.addLeds<LED_TYPE, LED_PIN, LED_ORDER>(leds, LED_MAX).setCorrection(TypicalLEDStrip);
  strip->setLeds(leds, LED_MAX);
  strip->clearLedData();
  // Выводим RGB во время загрузки
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Blue;
  strip->showLeds(50);
}

/***
 * Значение кнопки (true/false)
 */
bool checkButton() {
  uint32_t tmr = millis();
  while (millis() - tmr < 2000) {
    if (!digitalRead(BTN_PIN)) return true;
  }
  return false;
}

/***
 * Режим Access Point
 */
void accessPoint() {
  DEBUG("[AP] Access Point mode");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(GT_AP_SSID, GT_AP_PASS);
  myIP = WiFi.softAPIP();
  server.begin();
  fadeBlink(CRGB::Magenta);
  DEBUG("[AP] Done!");
}

/***
 * Подключение к роутеру
 */
void routerMode() {
  DEBUG("[STA] Router connection mode");
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(portalCfg.SSID, portalCfg.pass);
  uint32_t tmr = millis();
  bool state = false;
  while (millis() - tmr < 15000) {
    if (WiFi.status() == WL_CONNECTED) {
      fadeBlink(CRGB::Green);
      DEBUGLN("[STA] Connected successfully!");
      myIP = WiFi.localIP();
      return;
    }
    fader(CRGB::Yellow);
    yield();
  }
  fadeBlink(CRGB::Red);
  DEBUGLN("[STA] Failed to connect, fallback to AP mode!");
  accessPoint();
}
