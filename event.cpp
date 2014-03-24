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
  _newSubscriptionId = 0;
}

void EventSequencer::consumeEvents() {
  Event *e = _events.moveToHead();
  Subscription *s = _subscriptions.moveToHead();
  int i = 0;
  
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
      //if event source id and types match, call callback
      if (e->type == s->type && s->id == e->sourceId) {
        s->callback->call();
      }
      s = _subscriptions.moveToNext();
      i++;
    }
    
    e = _events.moveToNext();
  }
  
  //delete all events
  _events.empty(); 
  
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
  _subscriptionId = -1;
}

void Eventable::setSubscriptionId(int id) {
  _subscriptionId = id;
}

int Eventable::getSubscriptionId() {
  return _subscriptionId;
}

void Eventable::trigger(EventType type) {
  _evSeq->trigger(this, type);
}

void Eventable::unbind(EventType type) {
  _evSeq->unbind(this, type);
}

