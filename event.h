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
  EVT_NULL, EVT_BTN_CLICK, EVT_BTN_DBL_CLICK, EVT_SECOND_TICK
};

struct abstractCallback { 
 virtual void call() = 0;
 virtual ~abstractCallback() {}
};

//see here for example
//http://stackoverflow.com/questions/3381829/how-do-i-implement-a-callback-in-c
template <typename Destination>
struct EventableCallback : public abstractCallback {
 virtual void call() {((*destination).*member)(); }
 void (Destination::*member)();
 Destination *destination;
 EventableCallback(void (Destination::*m)(), Destination *p) : 
   member(m),
   destination(p)
 {}
 virtual ~EventableCallback() {

 }
};

struct Event {
  EventType type;
  int sourceId;
};

struct Subscription {
  void addCallback(abstractCallback* cb) {
    callback = cb;
  }
  abstractCallback* callback;
  EventType type;
  int id;
  ~Subscription() {
    delete callback;
  }
};

//stores subscriptions and triggers
//consumes trigger events by cross-referencing event type and list of subscriptions

class EventSequencer {
  public:
    EventSequencer(void);
    ~EventSequencer(void);
    void initialize(void);
    template <class Source, class Destination>
    void bind(Source* source, EventType type, void (Destination::*cb)(), Destination* destination);
      //creates a subscription object and pushes it into the subscription list
    template <class Source>
    void unbind(Source* source, EventType type);
    template <class Source>
    void trigger(Source* source, EventType type);
      //creates an event object and adds into the event list
    void consumeEvents();
    void clearEvents();
    void clearSubscriptions();
    void enable();
    void disable();
  private:
    List<Subscription> _subscriptions;
    List<Event> _events;
    int _newSubscriptionId;
    unsigned char _enabled;
};

class Eventable {
  public:
    Eventable();
    virtual ~Eventable(void);
    void initialize(EventSequencer* evSeq);
    template <class Destination>
    void bind(EventType type, void (Destination::*cb)());
    void unbind(EventType type);
    void trigger(EventType type);
    int getSubscriptionId();
    void setSubscriptionId(int id);
  protected:
    EventSequencer* _evSeq;
  private:
    int _subscriptionId;
};

//member template defs

template <class Source, class Destination>
void EventSequencer::bind(Source* source, EventType type, void (Destination::*cb)(), Destination* destination) {
  
  int subscriptionId = source->getSubscriptionId();
  
  abstractCallback* evCb = new EventableCallback< Destination > (cb, destination);
  
  if (subscriptionId < 0) {
    subscriptionId = _newSubscriptionId;
    source->setSubscriptionId(subscriptionId);
    _newSubscriptionId++;
  }
  
  Subscription* newSub = new Subscription;
  newSub->id = subscriptionId;
  newSub->type = type;
  newSub->addCallback(evCb);
  _subscriptions.pushNode(newSub);
  
}

template <class Source>
void EventSequencer::unbind(Source* source, EventType type) {
  Subscription* s = _subscriptions.moveToHead();
  
  while(s != 0) {
    if (source->getSubscriptionId() == s->id && type == s->type) {
      _subscriptions.spliceCurrent();
    }
    s = _subscriptions.moveToNext();
  }
  
}

template <class Source>
void EventSequencer::trigger(Source* source, EventType type) {
  if (_enabled == 0) {
    return;
  }
  Event* newEv = new Event;
  newEv->type = type;
  newEv->sourceId = source->getSubscriptionId();
  _events.pushNode(newEv);
}

template <class Destination>
void Eventable::bind(EventType type, void (Destination::*cb)()) {
  _evSeq->bind(this, type, cb, (Destination*)this);
}

#endif
