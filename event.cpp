#include "list.h"
#include "event.h"

/*************************************/
/* Event Sequencer
/*************************************/

EventSequencer::EventSequencer () {

}

EventSequencer::~EventSequencer () {

}

void EventSequencer::initialize () {
  _enabled = 1;
}

void EventSequencer::bind(Eventable* source, EventType type, void (Eventable::*cb)(), Eventable* destination) {
  
  abstractCallback* evCb = new EventableCallback(cb, destination);
  
  Subscription* newSub = new Subscription;
  newSub->source = source;
  newSub->type = type;
  newSub->addCallback(evCb);
  _subscriptions.insertAtRoot(newSub);
  
}

void EventSequencer::unbind(Eventable* source, EventType type) {
  Subscription* subscription = _subscriptions.moveToHead();
  
  while(subscription != 0) {
    if (source == subscription->source && type == subscription->type) {
      _subscriptions.spliceCurrent();
    }
    subscription = _subscriptions.moveToNext();
  }
  
}

void EventSequencer::trigger(Eventable* source, EventType type) {
  if (_enabled == 0) {
    return;
  }
  Event* newEv = new Event;
  newEv->type = type;
  newEv->source = source;
  _events.insertAtRoot(newEv);
}


void EventSequencer::consumeEvents() {
  Event *e = _events.moveToHead();
  Subscription *s = _subscriptions.moveToHead();
  
  if (_enabled == 0) {
    return;
  }
  
  if (e == 0) {
    return;
  }

  //no events
  if (s == 0) {
    return;
  }

  //loop through the list of events  
  while (e != 0) {
    s = _subscriptions.moveToHead();
    //loop through the list of subscriptions for all matching this event type
    while (s != 0) {
      //if types match, call callback
      if (e->type == s->type) {
        s->callback->call();
      }
      s = _subscriptions.moveToNext();
    }
    
    e = _events.moveToNext();
  }
  
  //delete all events
  clearEvents(); 
  
}

void EventSequencer::clearEvents() {
  _events.empty(); 
}

void EventSequencer::clearSubscriptions() {
  _subscriptions.empty(); 
}

int EventSequencer:: getNumSubscriptions() {
  return _subscriptions.getLength();
}

void EventSequencer::enable() {
  _enabled = 1;
}

void EventSequencer::disable() {
  _enabled = 0;  
}




/*************************************/
/* Eventable
/*************************************/

Eventable::Eventable () {
}

Eventable::~Eventable () {

}

void Eventable::initialize (EventSequencer* evSeq) {
  _evSeq = evSeq;
}

void Eventable::bind(EventType type, void (Eventable::*cb)()) {
  _evSeq->bind(this, type, cb, this);
}


void Eventable::trigger(EventType type) {
  _evSeq->trigger(this, type);
}

void Eventable::unbind(EventType type) {
  _evSeq->unbind(this, type);
}

