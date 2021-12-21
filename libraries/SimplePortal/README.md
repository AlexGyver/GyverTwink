![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# SimplePortal
Простой менеджер WiFi для esp8266 для задания логина-пароля WiFi и режима работы

### Совместимость
esp8266

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **SimplePortal** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/SimplePortal/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
Нет

<a id="usage"></a>
## Использование
```cpp
void portalStart();     // запустить портал
void portalStop();      // остановить портал
bool portalTick();      // вызывать в цикле
void portalRun(uint32_t prd = 60000);   // блокирующий вызов
byte portalStatus();    // статус: 1 connect, 2 ap, 3 local, 4 exit, 5 timeout

// константы статуса
SP_ERROR            // ошибка
SP_SUBMIT           // отправлены логин-пароль
SP_SWITCH_AP        // сигнал на смену в AP
SP_SWITCH_LOCAL     // сигнал на смену в Local
SP_EXIT             // нажата кнопка выход
SP_TIMEOUT          // вышел таймаут
```

### Хранение настроек
Из скетча доступна переменная (структура) **portalCfg**:
- `char* SSID[32]` - логин
- `char* pass[32]` - пароль
- `byte mode` - (1 `WIFI_STA`, 2 `WIFI_AP`)
После отправки данных с портала можно забрать информацию из структуры и/или записать её в EEPROM.

### Как работает
![demo](/doc/demo.png)
- Библиотека запускает режим **SoftAP**, запускает **DNSServer** и **ESP8266WebServer**.
- Коннектимся к точке, по умолчанию называется *"ESP Config"*
- Откроется страница конфигурации (если не открылась - перейди по *192.168.1.1*). На странице можно 
настроить режим работы ESP, а также сконфигурировать подключение к роутеру (логин-пароль).
- Кнопка *Submit* отправляет введённые данные на ESP. После нажатия *Submit* режим в конфиге переключается в 1 (WIFI_STA).
- Библиотека не переключает итоговый режим работы esp, кнопки портала *Switch AP* / *Switch Local* просто дают сигналы "в скетч".
- Кнопка Exit просто завершает работу портала.
- После нажатия любой кнопки или выхода таймаута портал закрывается, сервер останавливается, SoftAP отключается.

<a id="example"></a>
## Примеры
### Блокирующий
```cpp
#include <ESP8266WiFi.h>
#include <SimplePortal.h>

void setup() {
  Serial.begin(9600);
  delay(3000);
  
  portalRun();  // запустить с таймаутом 60с
  //portalRun(30000); // запустить с кастомным таймаутом
  
  Serial.println(portalStatus());
  // статус: 0 error, 1 connect, 2 ap, 3 local, 4 exit, 5 timeout
  
  if (portalStatus() == SP_SUBMIT) {
    Serial.println(portalCfg.SSID);
    Serial.println(portalCfg.pass);
    // забираем логин-пароль
  }
}

void loop() {
}
```

### Асинхронный
```cpp
#include <ESP8266WiFi.h>
#include <SimplePortal.h>

void setup() {
  Serial.begin(9600);

  // запускаем портал
  portalStart();
}

void loop() {
  // вызываем в loop
  if (portalTick()) {
    Serial.println(portalStatus());
    if (portalStatus() == SP_SUBMIT) {
      Serial.println(portalCfg.SSID);
      Serial.println(portalCfg.pass);
      // забираем логин-пароль
    }

    // сработает однократно при действии
    // точка будет автоматически выключена
  }
}
```

<a id="versions"></a>
## Версии
- v1.0

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!