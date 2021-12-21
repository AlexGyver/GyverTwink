#include <ESP8266WiFi.h>
#include <SimplePortal.h>

void setup() {
  Serial.begin(9600);

  // запускаем портал
  portalStart();
}

void loop() {
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
