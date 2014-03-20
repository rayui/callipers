//debouncer

/*
  debouncer.h - library to control 4 digit 7 segment LED
  (c) R Brooks 2014
*/
#ifndef Debouncer_h
#define Debouncer_h

#include <Energia.h>
#include "utils.h"
#include "list.h"
#include "event.h"

#define CTRL_PIN_PRESS LOW
#define CTRL_PIN_RELEASE HIGH

#define CLICK_BOUNCE_TIME 100
#define CLICK_TIMEOUT 250
#define EVENT_IGNORE_THRESHOLD 1000

struct ButtonEvent {
  int pin;
  long time;
};

template <class Base>
class Debouncer : public Base {
  public:
    Debouncer(int pin, EventSequencer* evSeq);
    ~Debouncer();
    void addEvent(ButtonEvent* e);
    void debounce();
    void onDoubleClick();
  private:
    List<ButtonEvent>* _events;
    EventSequencer* _evSeq;
    int _pin;
    long _lastPrimaryClickTime;
    int _primaryClickFlag;
    void _addEvent(ButtonEvent* e);
    void _debounce();
    void _triggerPrimaryClick(void);
};

#include "debouncer_impl.h"

#endif
