/*
  Скетч к проекту "GyverTwink"
  - Страница проекта (схемы, описания): https://alexgyver.ru/gyvertwink/
  - Исходники на GitHub: https://github.com/AlexGyver/GyverTwink
  Проблемы с загрузкой? Читай гайд для новичков: https://alexgyver.ru/arduino-first/
  AlexGyver, AlexGyver Technologies, 2021
*/

/*
  Мигает синим - открыт портал
  Мигает жёлтым - подключаемся к точке
  Мигнул зелёным - подключился к точке
  Мигнул красным - ошибка подключения к точке
  Мигнул розовым - создал точку
*/

// ================ НАСТРОЙКИ ================
#define BTN_PIN D3      // Пин кнопки
#define LED_PIN D1      // Пин ленты
#define LED_TYPE WS2812 // Чип ленты
#define LED_ORDER GRB   // Порядок цветов ленты
#define LED_MAX 500     // Максимальное число светодиодов
#define GT_AP_SSID "GyverTwink" // Название WiFi в режиме AP
#define GT_AP_PASS "12345678"   // Пароль WiFi в режиме AP

//#define DEBUG_SERIAL_GT // Раскомментируй, чтобы включить отладку

// ================== LIBS ==================
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SimplePortal.h>
#include <FastLED.h>
#include <EEManager.h>
#include <EncButton.h>
#include "palettes.h"
#include "Timer.h"

// ================== OBJECTS ==================
WiFiServer server(80);
WiFiUDP udp;
EEManager EEwifi(portalCfg);
CRGB leds[LED_MAX];
CLEDController *strip;
EncButton<EB_TICK, BTN_PIN> btn;
IPAddress myIP;

// ================== CONFIGURATION ==================
struct Cfg {
  uint16_t ledAm = 50; // Количество
  bool power = 1;      // Лента включена/выключена
  byte bright = 100;   // Яркость
  bool autoCh = 0;     // Автоматически менять эффект
  bool rndCh = 0;      // Случайный эффект
  byte prdCh = 1;      // Время таймера смены режимов
  bool turnOff = 0;    // Включен ли таймер выключения
  byte offTmr = 60;    // Время таймер выключения
};
Cfg cfg;
EEManager EEcfg(cfg);

byte xy[LED_MAX][2];
EEManager EExy(xy);

struct MM {
  byte minY = 0;
  byte maxY = 255;
  byte minX = 0;
  byte maxX = 255;
  byte w = 255;
  byte h = 255;
};
MM mm;
EEManager EEmm(mm);

#define ACTIVE_PALETTES 11
struct Effects {
  bool fav = true;
  byte scale = 50;
  byte speed = 150;
};
Effects effs[ACTIVE_PALETTES * 2];
EEManager EEeff(effs);

// ================== MISC DATA ==================
Timer forceTmr(30000, false); // Таймер принудительного выбранного эффекта
Timer switchTmr(0, false);    // Таймер авто-смены эффекта
Timer offTmr(60000, false);   // Таймер авто-выключения
bool calibF = false; // Режим калибровки
byte curEff = 0;     // Текущий эффект
byte forceEff = 0;   // Принудительно выбранный эффект

#ifdef DEBUG_SERIAL_GT
#define DEBUGLN(x) Serial.println(x)
#define DEBUG(x) Serial.print(x)
#else
#define DEBUGLN(x)
#define DEBUG(x)
#endif

// ================== SETUP ==================
void setup() {
#ifdef DEBUG_SERIAL_GT
  Serial.begin(115200);
  DEBUGLN();
#endif
  DEBUG("[Setup] Starting up LEDs");
  startLEDs();
  EEPROM.begin(2048); // С запасом!
  
  // Если это первый запуск или нажатие по кнопке,
  // то заходим в режим настройки
  if (EEwifi.begin(0, 'a') || checkButton()) configMode();

  // Если мы в режиме Access Point, или мы в режиме Router (STA) и SSID пустой, то запускаем Access Point
  if (portalCfg.mode == WIFI_AP || (portalCfg.mode == WIFI_STA && portalCfg.SSID[0] == '\0')) accessPoint();
  else routerMode(); // Если нет, запускаем режим Router (STA)

  DEBUG("[Setup] IP address: ");
  DEBUGLN(myIP);

  DEBUG("[Setup] Initializing configuration");
  EEcfg.begin(EEwifi.nextAddr(), 'a');
  EEeff.begin(EEcfg.nextAddr(), 'a');
  EEmm.begin(EEeff.nextAddr(), (uint8_t)LED_MAX);
  EExy.begin(EEmm.nextAddr(), (uint8_t)LED_MAX);

  DEBUG("[Setup] Loading configuration");
  switchTmr.setPrd(cfg.prdCh * 60000ul);
  if (cfg.autoCh) switchTmr.restart();
  switchEffect();
  cfg.turnOff = false;
  strip->setLeds(leds, cfg.ledAm);
  DEBUG("[Setup] Starting UDP server");
  udp.begin(8888);
  DEBUG("[Setup] Done!");
}

// ================== LOOP ==================
void loop() {
  handleButton(); // опрос кнопки

  // Менеджер EEPROM
  EEcfg.tick();
  EEeff.tick();

  parseUDP(); // Парсим UDP пакеты

  // Таймер принудительного показа эффектов
  if (forceTmr.ready()) {
    forceTmr.stop();
    switchEffect();
  }

  // Принудительный показ эффектов выключен, меняем эффект
  if (!forceTmr.state() && switchTmr.ready()) switchEffect();

  // Таймер выключения
  if (offTmr.ready()) {
    DEBUGLN("[OffTimer] Shutting down");
    offTmr.stop();
    cfg.turnOff = false;
    cfg.power = false;
    strip->showLeds(0);
    EEcfg.update();
  }

  // Показываем эффект, если лента включена
  if (!calibF && cfg.power) handleEffects();
}
