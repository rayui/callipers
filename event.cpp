#include "list.h"
#include "event.h"

/*************************************/
/* Event Sequencer
/*************************************/

EventSequencer::EventSequencer () {
  _subscriptions = new List<Subscription>;
  _events = new List<Event>;
  _newSubscriptionId = 0;
}

EventSequencer::~EventSequencer () {
  delete _subscriptions;
  delete _events;
}

void EventSequencer::consumeEvents() {
  Event *e = _events->moveToHead();
  Subscription *s = _subscriptions->moveToHead();
  int i = 0;
  
  if (e == 0) {
    return;
  }

  //no events
  if (s == 0) {
    return;
  }

  Serial << "Subscription list length: " << _subscriptions->getLength() << ", size of data: " << _subscriptions->getLength() * sizeof(Subscription) << "\n";

  //loop through the list of events  
  while (e != 0) {
    Serial << "Consuming event of type: " << e->type << " and subscription id " << e->sourceId << "\n";
    s = _subscriptions->moveToHead();
    //loop through the list of subscriptions for all matching this event type
    while (s != 0) {
      Serial << "Comparing subscription# " << i << " of type: " << s->type << " and subscription id " << s->id << "\n";
      //if event source id and types match, call callback
      if (e->type == s->type && s->id == e->sourceId) {
        Serial << "GOT A MATCH\n";
        s->callback->call();
      }
      s = _subscriptions->moveToNext();
      i++;
    }
    
    Serial << "Finished matching subscriptions\n";
    
    e = _events->moveToNext();
  }
  
  //delete all events
  _events->empty(); 
  
}


/*************************************/
/* Eventable
/*************************************/

Eventable::Eventable (EventSequencer* evSeq) {
  _evSeq = evSeq;
  _subscriptionId = -1;
}

Eventable::~Eventable () {

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

