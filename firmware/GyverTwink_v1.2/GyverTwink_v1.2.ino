/*
  Скетч к проекту "GyverTwink"
  - Страница проекта (схемы, описания): https://alexgyver.ru/gyvertwink/
  - Исходники на GitHub: https://github.com/AlexGyver/GyverTwink
  Проблемы с загрузкой? Читай гайд для новичков: https://alexgyver.ru/arduino-first/
  AlexGyver, AlexGyver Technologies, 2021
*/

/*
  1.1 - исправлена калибровка больше 255 светодиодов
  1.2 - исправлена ошибка с калибровкой
  1.3 - https://github.com/AlexGyver/GyverTwink/pull/6
*/

/*
  Мигает синим - открыт портал
  Мигает жёлтым - подключаемся к точке
  Мигнул зелёным - подключился к точке
  Мигнул красным - ошибка подключения к точке
  Мигнул розовым - создал точку
*/

// ============================================================
// Настройки
#define BTN_PIN D3      // Пин кнопки
#define BTN_TOUCH 0     // 1 - сенсорная кнопка, 0 - нет

#define LED_PIN D1      // Пин ленты
#define LED_TYPE WS2812 // Чип ленты
#define LED_ORDER GRB   // Порядок цветов ленты
#define LED_MAX 500     // Максимальное количество светодиодов

// Настройки сети в режиме Access Point (Точка доступа)
#define GT_AP_SSID "GyverTwink" // Название сети
#define GT_AP_PASS "12345678"   // Пароль сети
#define GT_AP_CHANNEL 1         // Канал сети (лучше не трогать)
#define GT_AP_HIDDEN false      // Скрытая сеть (false - нет, true - да)
#define GT_AP_MAX 4             // Максимум подключений 

// Для включения отладки расскоментируй строчку ниже
//#define DEBUG_SERIAL_GT

// ============================================================
// Библиотеки
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SimplePortal.h>
#include <FastLED.h>
#include <EEManager.h>
#include <EncButton.h>
#include "palettes.h"
#include "Timer.h"

// ============================================================
// Переменные
WiFiServer server(80);           // Подключение к WiFi
WiFiUDP udp;                     // Сервер UDP на порте 8888
EEManager EEwifi(portalCfg);     // Корнфигурация подключения
CRGB leds[LED_MAX];              // Массив из всех LED
CLEDController *strip;           // Вся LED лента
EncButton<EB_TICK, BTN_PIN> btn; // Кнопка
IPAddress currentIP;             // IP адрес
bool calibrationMode = false;    // Режим калибровки
byte currentEffect = 0;          // Текущий эффект
byte forcedEffect = 0;           // Принудительный запуск эффекта

// ============================================================
// Конфигурация
struct Cfg {
  uint16_t ledAmount = 50;
  bool power = 1;
  byte brightness = 100;
  bool autoChange = 0;
  bool randomChange = 0;
  byte changePeriod = 1;
  bool turnOff = 0;
  byte turnOffTimer = 60;
};
Cfg cfg;              // Конфигурация
EEManager EEcfg(cfg); // Менеджер

byte xy[LED_MAX][2]; // Два байта для X/Y позиции
                     // (откалиброванные значения)
EEManager EExy(xy);  // Менеджер

struct MM {
  byte minY = 0;
  byte maxY = 255;
  byte minX = 0;
  byte maxX = 255;
  byte w = 255;
  byte h = 255;
};
MM mm;              // Откалиброванные значения
EEManager EEmm(mm); // Менеджер

#define ACTIVE_PALETTES 11
struct Effects {
  bool favourite = true;
  byte scale = 50;
  byte speed = 150;
};
Effects effects[ACTIVE_PALETTES * 2]; // Настройки всех эффектов
EEManager EEeff(effects);             // Менеджер

// ============================================================
// Таймеры (все выключены по дефолту)
Timer forceTimer(30000, false);
Timer switchTimer(0, false);
Timer turnOffTimer(60000, false);

// ============================================================
// Отладка
#ifdef DEBUG_SERIAL_GT
#define DEBUGLN(x) Serial.println(x)
#define DEBUG(x) Serial.print(x)
#else
#define DEBUGLN(x)
#define DEBUG(x)
#endif

// ============================================================
// Запуск
void setup() {
  // Врубаем отладку, если нужно
#ifdef DEBUG_SERIAL_GT
  Serial.begin(115200);
  DEBUGLN();
#endif
  delay(200);
  if (BTN_TOUCH) btn.setButtonLevel(HIGH);
  startStrip(); // Запускаем LED ленту
  EEPROM.begin(2048); // С запасом!

  // если это первый запуск или щелчок по кнопке, открываем портал
  if (EEwifi.begin(0, 'a') || buttonState()) setupNetwork();

  // Если мы в режиме Access Point, или же мы в режиме подключения к роутеру и название сети пустое,
  if (portalCfg.mode == WIFI_AP || (portalCfg.mode == WIFI_STA && portalCfg.SSID[0] == '\0')) 
    accessPoint(); // То включаем режим Access Point (точка доступа)
  else routerConnect(); // Если нет, врубаем режим подключения к роутеру
  
  DEBUG("[Setup] IP address: ");
  DEBUGLN(currentIP);

  EEcfg.begin(EEwifi.nextAddr(), 'a');            // Грузим конфигурацию
  EEeff.begin(EEcfg.nextAddr(), 'a');             // Гружим эффекты
  EEmm.begin(EEeff.nextAddr(), (uint8_t)LED_MAX); // Грузим откалиброванные значения
  EExy.begin(EEmm.nextAddr(), (uint8_t)LED_MAX);  // Грузим позиции LEDs

  switchTimer.setPrd(cfg.changePeriod * 60000ul); // Инитиализируем аймер авто-смены
  if (cfg.autoChange) switchTimer.restart();      // Запускаем таймер авто-смены
  switchEffect();                           // Меняем эффект
  cfg.turnOff = false;                   // Вырубаем таймер авто-выключения
  strip->setLeds(leds, cfg.ledAmount);   // Ставим количество LED
  udp.begin(8888);                       // Запускаем UDP сервер
}

// ============================================================
// Цикл
void loop() {
  handleButton(); // Читаем кнопку

  // Менеджер EEPROM
  EEcfg.tick();
  EEeff.tick();

  handlePackets(); // Читаем пакеты UDP

  // Таймер принудительного показа эффектов
  if (forceTimer.ready()) {
    forceTimer.stop();
    switchEffect();
  }

  // Принудительного показ выключен, меняем эффект
  if (!forceTimer.state() && switchTimer.ready()) switchEffect();

  // Таймер авто-выключения
  if (turnOffTimer.ready()) {
    turnOffTimer.stop();
    cfg.turnOff = false;
    cfg.power = false;
    strip->showLeds(0);
    EEcfg.update();
    DEBUGLN("[Turn Off Timer] Disabling LEDs!");
  }

  // Показываем эффект, если лента включена
  if (!calibrationMode && cfg.power) playEffects();
}
