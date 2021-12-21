/*
    Менеджер EEPROM - библиотека для уменьшения износа памяти
    Документация: 
    GitHub: https://github.com/GyverLibs/EEManager
    Возможности:
    - Отложенная запись (обновление) по таймеру
    - Работает на базе стандартной EEPROM.h
    - Встроенный механизм ключа первой записи

    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
    v1.1 - изменены коды возврата begin
    v1.2 - добавлена nextAddr()
*/

#ifndef _EEManager_h
#define _EEManager_h
#include <Arduino.h>
#include <EEPROM.h>

class EEManager {
public:
    // передать данные любого типа, опционально таймаут обновления в мс
    template <typename T> EEManager(T &data, uint16_t tout = 5000) {
        _data = (uint8_t*) &data;
        _size = sizeof(T);
        _tout = tout;
    }
    
    // сменить таймаут
    void setTimeout(uint16_t tout = 5000) {
        _tout = tout;
    }
    
    // начать работу, прочитать данные в переменную. Принимает адрес начала хранения даты и ключ
    uint8_t begin(uint8_t addr, uint8_t key) {        
        _addr = addr;
        if (_addr + _size + 1 > EEPROM.length()) return 2;  // не хватит места
        _ready = 1;
        if (EEPROM.read(_addr + _size) != key) {            // ключ не совпал
            EEPROM.write(_addr + _size, key);               // пишем ключ
            updateNow();                                    // пишем стандартные значения
            return 1;
        }
        for (uint16_t i = 0; i < _size; i++) _data[i] = EEPROM.read(_addr + i);
        return 0;
    }
    
    // обновить данные в еепром сейчас
    void updateNow() {
        if (_ready) {            
#ifdef ESP8266
            for (uint16_t i = 0; i < _size; i++) EEPROM.write(_addr + i, _data[i]);
            EEPROM.commit();
#else
            for (uint16_t i = 0; i < _size; i++) EEPROM.update(_addr + i, _data[i]);
#endif
        }
    }
    
    // отложить обновление и сбросить таймер
    void update() {
        _tmr = millis();
        _update = 1;
    }
    
    // тикер обновления
    bool tick() {
        if (_update && millis() - _tmr >= _tout) {
            updateNow();
            _update = 0;
            return 1;
        } return 0;
    }
    
    // сбросить ключ запуска. При перезагрузке (или вызове begin) запишутся стандартные данные 
    void reset() {
        EEPROM.write(_addr + _size, EEPROM.read(_addr + _size) + 1);   // меняем ключ на +1, при перезапуске будет дефолт
    }
    
    // получить размер данных
    uint16_t dataSize() {
        return _size;
    }
    
    // получить размер всего блока (данные + ключ)
    uint16_t blockSize() {
        return _size + 1;
    }
    
    // получить адрес первого байта в блоке
    uint16_t startAddr() {
        return _addr;
    }
    
    // получить адрес последнего байта в блоке (включая ключ)
    uint16_t endAddr() {
        return _addr + _size;
    }
    
    // получить первый свободный адрес для следующего блока
    uint16_t nextAddr() {
        return _addr + _size + 1;
    }

private:
    uint8_t* _data;
    uint16_t _size, _addr;
    bool _ready = 0, _update = 0;
    uint32_t _tmr = 0;
    uint16_t _tout;
};

#endif