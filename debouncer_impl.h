template <class Base> Debouncer<Base>::Debouncer (int pin, EventSequencer* evSeq) : Base(evSeq) {
  _pin = pin;
  _events = new List<ButtonEvent>;
  _lastPrimaryClickTime = 0;
  _primaryClickFlag = 0;
}

template <class Base> void Debouncer<Base>::_addEvent(ButtonEvent* event) {
  ButtonEvent* e = new ButtonEvent;
  e->pin = event->pin;
  e->time = event->time;
  _events->pushNode(e);
}

template <class Base> void Debouncer<Base>::_debounce() { 
  int timeDiff = 0;
  ButtonEvent* current;
  
  if (_events->getRoot() == 0) {
    return;
  }
  
  current = _events->moveToHead();
  
  while(current != 0) {
    if (current->pin == _pin) {
      timeDiff = current->time - _lastPrimaryClickTime;
      if (timeDiff > CLICK_BOUNCE_TIME) {
        if (timeDiff < CLICK_TIMEOUT && _primaryClickFlag != 0) {
          Base::trigger(EVT_BTN_DBL_CLICK);
          _primaryClickFlag = 0;
        } else {
          _primaryClickFlag = 1;
        }
        _lastPrimaryClickTime = current->time;
      }
          
      current = _events->moveToNext();
      _events->spliceRoot();
    }
  }
  
}

template <class Base> void Debouncer<Base>::addEvent(ButtonEvent* event) {
  if (event != 0) {
    _addEvent(event);
  }
}

template <class Base> void Debouncer<Base>::debounce() {
  _debounce();
  _triggerPrimaryClick();
}

template <class Base> void Debouncer<Base>::_triggerPrimaryClick() {
  if (millis() - _lastPrimaryClickTime > CLICK_TIMEOUT && _primaryClickFlag != 0) {
    Base::trigger(EVT_BTN_CLICK);
    _primaryClickFlag = 0;
  }
}

template <class Base> void Debouncer<Base>::onDoubleClick() {
  Serial << "DOUBLE CLICK. PIN IS " << _pin << "\n";
}
