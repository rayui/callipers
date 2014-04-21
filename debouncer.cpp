#include "debouncer.h"

Debouncer::Debouncer(EventSequencer* evSeq) : Eventable(evSeq) {
  _lastPrimaryClickTime = 0;
  _primaryClickFlag = 0;
}


void Debouncer::checkBounce(volatile long time) {
  if (time != 0) { 
    _lastEventTime = (long)time;
  }
}

void Debouncer::_debounce() { 
  long timeDiff = 0;
  
  if (_lastEventTime != -1) {
    timeDiff = _lastEventTime - _lastPrimaryClickTime;
    if (timeDiff > CLICK_BOUNCE_TIME) {
      if (timeDiff < CLICK_TIMEOUT && _primaryClickFlag != 0) {
        Eventable::trigger(EVT_BTN_DBL_CLICK, 0);
        _primaryClickFlag = 0;
      } else {
        _primaryClickFlag = 1;
      }
      _lastPrimaryClickTime = _lastEventTime;
    }
  }
 
  _lastEventTime = -1;
 
}

void Debouncer::debounce() {
  _debounce();
  _triggerPrimaryClick();
}

void Debouncer::_triggerPrimaryClick() {
  if (millis() - _lastPrimaryClickTime > CLICK_TIMEOUT && _primaryClickFlag != 0) {
    Eventable::trigger(EVT_BTN_CLICK, 0);
    _primaryClickFlag = 0;
  }
}
