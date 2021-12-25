#define MAX_UDP_PACKET 30
byte ubuf[MAX_UDP_PACKET];

// Отсылаем ответ
void reply(byte* data, byte size) {
  udp.beginPacket(udp.remoteIP(), udp.remotePort());
  udp.write("GT");
  udp.write(data, size);
  udp.endPacket();
}

// Парсим UDP пакеты
void handlePackets() {
  static uint32_t Timer;
  if (udp.parsePacket()) {
    if (millis() - Timer < 50) { // Антидребезг, 50 повторений игнорируем все
      udp.read(ubuf, MAX_UDP_PACKET);
      return;
    }
    Timer = millis();
    int n = udp.read(ubuf, MAX_UDP_PACKET);
    ubuf[n] = 0;
    if (ubuf[0] != 'G' || ubuf[1] != 'T') return;
    byte answ[10];

    switch (ubuf[2]) {
      case 0: // Запрос IP
        delay(currentIP[3] * 2);
        answ[0] = 0;
        answ[1] = currentIP[3];
        reply(answ, 2);
        break;

      case 1: // Запрос настроек
        answ[0] = 1;
        answ[1] = (byte)cfg.ledAmount;
        answ[2] = (byte)cfg.ledAmount >> 8;
        answ[3] = cfg.power;
        answ[4] = cfg.brightness;
        answ[5] = cfg.autoChange;
        answ[6] = cfg.randomChange;
        answ[7] = cfg.changePeriod;
        answ[8] = cfg.turnOff;
        answ[9] = cfg.turnOffTimer;
        reply(answ, 10);
        break;

      case 2: // Приём настроек
        forceTimer.stop();
        switch (ubuf[3]) {
          case 0: // Количество LEDs
            cfg.ledAmount = ubuf[4] * 100 + ubuf[5];
            strip->setLeds(leds, cfg.ledAmount);
            answ[0] = 2;
            answ[1] = 0;
            reply(answ, 2);
            break;
          case 1: // Лента включена/выключена
            cfg.power = ubuf[4];
            answ[0] = 2;
            answ[1] = 1;
            reply(answ, 2);
            break;
          case 2: // Яркость
            cfg.brightness = ubuf[4];
            answ[0] = 2;
            answ[1] = 2;
            reply(answ, 2);
            break;
          case 3: // Авто-смена включена/выключена
            cfg.autoChange = ubuf[4];
            if (cfg.autoChange) switchTimer.restart();
            else switchTimer.stop();
            answ[0] = 2;
            answ[1] = 3;
            reply(answ, 2);
            break;
          case 4: // Рандомная авто-смена включена/выключена
            cfg.randomChange = ubuf[4];
            answ[0] = 2;
            answ[1] = 4;
            reply(answ, 2);
            break;
          case 5: // Период авто-смены в минутах
            cfg.changePeriod = ubuf[4];
            switchTimer.setPrd(cfg.changePeriod * 60000ul);
            if (cfg.autoChange) switchTimer.restart();
            answ[0] = 2;
            answ[1] = 5;
            reply(answ, 2);
            break;
          case 6: // Следующий эффект
            switchEffect();
            if (cfg.autoChange) switchTimer.restart();
            answ[0] = 2;
            answ[1] = 6;
            reply(answ, 2);
            break;
          case 7: // Авто-выключение включена/выключена
            cfg.turnOff = ubuf[4];
            if (cfg.turnOff) turnOffTimer.restart();
            else turnOffTimer.stop();
            answ[0] = 2;
            answ[1] = 7;
            reply(answ, 2);
            break;
          case 8: // Период авто-выключения в минутах
            cfg.turnOffTimer = ubuf[4];
            turnOffTimer.setPrd(cfg.turnOffTimer * 60000ul);
            if (cfg.turnOff) turnOffTimer.restart();
            answ[0] = 2;
            answ[1] = 8;
            reply(answ, 2);
            break;
        }
        if (!cfg.power) strip->showLeds(0);
        EEcfg.update();
        break;

      case 3: // Калибровка
        switch (ubuf[3]) {
          case 0: // Начать
            calibrationMode = true;
            strip->clearLedData();
            strip->showLeds(0);
            DEBUGLN("[Calibration] Began calibration!");
            answ[0] = 3;
            answ[1] = 0;
            reply(answ, 2);
            break;
          case 1: { // Откалибровать предыдущий LED и перейти на следующий
            int currentLED = ubuf[4] * 100 + ubuf[5];
            if (currentLED > 0) {
              xy[currentLED - 1][0] = ubuf[6];
              xy[currentLED - 1][1] = ubuf[7];
            }
            strip->clearLedData();
            leds[currentLED] = CRGB::White;
            strip->showLeds(200);
            answ[0] = 3;
            answ[1] = 1;
            reply(answ, 2);
            break; 
          }
          case 2: // Завершить
            calibrationMode = false;
            strip->clearLedData();
            strip->showLeds(0);
            EExy.updateNow();

            mm.minY = 255;
            mm.maxY = 0;
            mm.minX = 255;
            mm.maxX = 0;
            for (int i = 0; i < cfg.ledAmount; i++) {
              mm.minX = min(mm.minX, xy[i][0]);
              mm.maxX = max(mm.maxX, xy[i][0]);
              mm.minY = min(mm.minY, xy[i][1]);
              mm.maxY = max(mm.maxY, xy[i][1]);
            }
            mm.w = mm.maxX - mm.minX;
            mm.h = mm.maxY - mm.minY;
            EEmm.updateNow();
            DEBUGLN("[Calibration] Finished!");
            answ[0] = 3;
            answ[1] = 2;
            reply(answ, 2);
            break;
        }
        break;

      case 4: // Управление эффектами
        forceTimer.restart();
        EEeff.update();
        switch (ubuf[3]) {
          case 0: // Выбор эффекта
            forcedEffect = ubuf[4];
            answ[0] = 4;
            answ[1] = 0;
            answ[2] = effects[forcedEffect].favourite;
            answ[3] = effects[forcedEffect].scale;
            answ[4] = effects[forcedEffect].speed;
            reply(answ, 4);
            break;
          case 1: // Флажок "Избранное"
            effects[currentEffect].favourite = ubuf[4];
            answ[0] = 4;
            answ[1] = 1;
            reply(answ, 2);
            break;
          case 2: // Меняем масштаб
            effects[currentEffect].scale = ubuf[4];
            answ[0] = 4;
            answ[1] = 2;
            reply(answ, 2);
            break;
          case 3: // Меняем скорость
            effects[currentEffect].speed = ubuf[4];
            answ[0] = 4;
            answ[1] = 3;
            reply(answ, 2);
            break;
        }
        break;
    }
  }
}
