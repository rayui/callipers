#include "displayable_manager.h"

DisplayableManager::DisplayableManager(EventSequencer* evSeq) : Eventable(evSeq) {
  appId = 0;
  _current = 0;
}

DisplayableManager::~DisplayableManager(void) {
  
}

void DisplayableManager::loadTemp() {
  _current = new Temp(_evSeq);
  _current->bind(EVT_SECOND_TICK, (memberPointer)&Temp::updateReading);
  _current->bind(EVT_BTN_CLICK, (memberPointer)&Temp::toggleScale);
  appId = 1;
}

void DisplayableManager::loadEncoder() {
  _current = new Encoder(_evSeq);
  _current->bind(EVT_ENCODER, (memberPointer)&Encoder::updateReading);
  appId = 0;
}

void DisplayableManager::deleteCurrent() {
  _current->unbindAll();
  delete _current;
  _current = 0;
}

void DisplayableManager::loadNextApp() {

  if (_current == 0) {
    loadEncoder();
    return;
  }

  deleteCurrent();

  if (appId == 1) {
    loadEncoder();
  } else {
    loadTemp();
  }

}

unsigned char DisplayableManager::getDPMask() {
  if (_current != 0) {
    return _current->getDPMask();    
  }
  
  return 0;
}

int DisplayableManager::toString(char* output) {
  if (_current != 0) {
    return _current->toString(output);
    itoa((int)_current, output, 16);
    return 4;
  }
 
  return 0;
}
