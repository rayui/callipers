#include "displayable_manager.h"

DisplayableManager::DisplayableManager(void) : Eventable() {
  
}

DisplayableManager::~DisplayableManager(void) {
  
}

void DisplayableManager::initialize (EventSequencer* evSeq) {
  Eventable::initialize(evSeq);
  _currentApp = 0;
}

void DisplayableManager::loadTemp() {
  _currentApp = new Temp();
  
  ((Temp*)_currentApp)->initialize(NUM_LED_DIGITS, _evSeq);
  _currentApp->bind(EVT_SECOND_TICK, &Temp::updateReading);
  _currentApp->bind(EVT_BTN_CLICK, &Temp::toggleScale);
  
}

void DisplayableManager::deleteTemp() {
  _currentApp->unbind(EVT_BTN_CLICK);
  _currentApp->unbind(EVT_SECOND_TICK);
  delete (Temp*)_currentApp;
  _currentApp = 0;
}

void DisplayableManager::loadNextApp() {
  _evSeq->disable();
  deleteTemp();
  loadTemp();
  _evSeq->enable();
}

unsigned char DisplayableManager::getDPMask() {
  if (_currentApp != 0) {
    return _currentApp->getDPMask();    
  }
  return 0;
}

int DisplayableManager::toString(char* output) {
  if (_currentApp != 0) {
    return _currentApp->toString(output);
  }
  return 0;
}
