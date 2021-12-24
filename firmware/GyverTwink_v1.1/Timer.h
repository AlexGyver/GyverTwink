// класс таймера
#pragma once
struct Timer {
  Timer (uint32_t prd, boolean state = true) {
    setPrd(prd);
    _state = state;
  }

  void setPrd(uint32_t prd) {
    if (prd > 0) _prd = prd;
    else stop();
  }

  bool ready() {
    if (_state && millis() - _tmr >= _prd) {
      restart();
      return true;
    }
    return false;
  }

  void restart() {
    _tmr = millis();
    _state = true;
  }

  void stop() {
    _state = false;
  }

  bool state() {
    return _state;
  }

  uint32_t _tmr, _prd;
  bool _state = true;
};
