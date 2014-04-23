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

typedef void* evtArg;
typedef void (Eventable::*memberPointer)(evtArg data);

struct abstractCallback { 
 virtual void call(evtArg data) = 0;
 virtual ~abstractCallback() {}
};

//see here for example
//http://stackoverflow.com/questions/3381829/how-do-i-implement-a-callback-in-c
struct EventableCallback : public abstractCallback {
  memberPointer member;
  Eventable *dest;
  EventableCallback(memberPointer m, Eventable *p) : member(m), dest(p) {} 
  virtual ~EventableCallback() {}
  virtual void call(evtArg data) {
    (dest->*member)(data);
  }
};

struct Event {
  EventType type;
  Eventable* source;
  evtArg data;
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
    void bind(Eventable* source, EventType type, memberPointer cb, Eventable* destination);
    void unbind(Eventable* source, EventType type);
    void unbindAll(Eventable* source);
    void trigger(Eventable* source, EventType type, evtArg data);
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
    virtual void bind(EventType type, memberPointer cb);
    virtual void unbind(EventType type);
    void unbindAll();
    virtual void trigger(EventType type, evtArg data);
  protected:
    EventSequencer* _evSeq;
  private:
};


#endif
