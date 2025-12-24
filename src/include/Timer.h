#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
  private:
    unsigned long _prevMillis;
    unsigned long _interval;

  public:
    Timer(unsigned long interval) {
      _interval = interval;
      _prevMillis = 0;
    }

    bool isReady() {
      unsigned long curr = millis();
      if (curr - _prevMillis >= _interval) {
        _prevMillis = curr;
        return true;
      }
      return false;
    }

    void reset() {
      _prevMillis = millis();
    }
};

#endif