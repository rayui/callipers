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

#define CTRL_PIN_PRESS HIGH
#define CTRL_PIN_RELEASE LOW

#define CLICK_BOUNCE_TIME 100
#define CLICK_TIMEOUT 250
#define EVENT_IGNORE_THRESHOLD 1000

struct ButtonEvent {
  int pin;
  long time;
};

class Debouncer : public Eventable {
  public:
    void initialize(int pin, EventSequencer* evSeq);
    void addEvent(volatile ButtonEvent* e);
    void debounce();
  private:
    List<ButtonEvent>* _events;
    EventSequencer* _evSeq;
    int _pin;
    long _lastPrimaryClickTime;
    int _primaryClickFlag;
    void _addEvent(volatile ButtonEvent* e);
    void _debounce();
    void _triggerPrimaryClick(void);
};

#endif
