//event mixin

/*
  leddisplay.h - library to control 4 digit 7 segment LED
  (c) R Brooks 2014
*/
#ifndef Event_h
#define Event_h

#include <Energia.h>

#include "list.h"

enum EventType {
  EVT_NULL, EVT_BTN_CLICK, EVT_BTN_DBL_CLICK, EVT_SECOND_TICK, EVT_ENCODER
};

//forward declare eventable type
class Eventable;

typedef void (Eventable::*memberPointer)();

struct abstractCallback { 
 virtual void call() = 0;
 virtual ~abstractCallback() {}
};

//see here for example
//http://stackoverflow.com/questions/3381829/how-do-i-implement-a-callback-in-c
struct EventableCallback : public abstractCallback {
 virtual void call() {((*destination).*member)(); }
 void (Eventable::*member)();
 Eventable *destination;
 EventableCallback(void (Eventable::*m)(), Eventable *p) : 
   member(m),
   destination(p)
 {}
 virtual ~EventableCallback() {

 }
};

struct Event {
  EventType type;
  Eventable* source;
  int data;
};

struct Subscription {
  void addCallback(abstractCallback* cb) {
    callback = cb;
  }
  Eventable* source;
  abstractCallback* callback;
  EventType type;
  virtual ~Subscription() {
    delete callback;
  }
};

//stores subscriptions and triggers
//consumes trigger events by cross-referencing event type and list of subscriptions

class EventSequencer {
  public:
    EventSequencer(void);
    ~EventSequencer(void);
    void bind(Eventable* source, EventType type, void (Eventable::*cb)(), Eventable* destination);
    void unbind(Eventable* source, EventType type);
    void unbindAll(Eventable* source);
    void trigger(Eventable* source, EventType type, int data);
    void consumeEvents();
    void clearEvents();
    void clearSubscriptions();
    int getNumSubscriptions();
    void enable();
    void disable();
  private:
    List<Subscription> _subscriptions;
    List<Event> _events;
    unsigned char _enabled;
};

class Eventable {
  public:
    Eventable(EventSequencer* evSeq);
    virtual ~Eventable(void);
    virtual void bind(EventType type, void (Eventable::*cb)());
    virtual void unbind(EventType type);
    void unbindAll();
    virtual void trigger(EventType type, int data);
  protected:
    EventSequencer* _evSeq;
  private:
};


#endif
