#include "list.h"
#include "event.h"

/*************************************/
/* Event Sequencer
/*************************************/

EventSequencer::EventSequencer () {
  _enabled = 1;
}

EventSequencer::~EventSequencer () {

}

void EventSequencer::bind(Eventable* source, EventType type, memberPointer cb, Eventable* destination) {
  
  abstractCallback* evCb = new EventableCallback(cb, destination);
  
  Subscription* newSub = new Subscription;
  newSub->source = source;
  newSub->type = type;
  newSub->addCallback(evCb);
  _subscriptions.pushNode(newSub);
  
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


void EventSequencer::unbindAll(Eventable* source) {
  Subscription* subscription = _subscriptions.moveToHead();
  
  while(subscription != 0) {
    if (source == subscription->source) {
      _subscriptions.spliceCurrent();
    }
    subscription = _subscriptions.moveToNext();
  }

}


void EventSequencer::trigger(Eventable* source, EventType type, evtArg data) {
  if (_enabled == 0) {
    return;
  }
  Event* newEv = new Event;
  newEv->type = type;
  newEv->source = source;
  newEv->data = data;
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
        s->callback->call(e->data);
      }
      s = _subscriptions.moveToNext();
    }
    
    e = _events.moveToNext();
  }
  
  //delete all events
  clearEvents(); 
  
}

void EventSequencer::clearEvents() {
  disable();
  _events.empty();
  enable(); 
}

void EventSequencer::clearSubscriptions() {
  disable();
  _subscriptions.empty();
  enable();
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

Eventable::Eventable (EventSequencer* evSeq) {
  _evSeq = evSeq;
}

Eventable::~Eventable () {
  unbindAll();
}

void Eventable::bind(EventType type, memberPointer cb) {
  _evSeq->bind(this, type, cb, this);
}


void Eventable::trigger(EventType type, evtArg data) {
  _evSeq->trigger(this, type, data);
}

void Eventable::unbind(EventType type) {
  _evSeq->unbind(this, type);
}

void Eventable::unbindAll() {
  _evSeq->unbindAll(this);
}
