#include "displayable_manager.h"

DisplayableManager::DisplayableManager(EventSequencer* evSeq) : Eventable(evSeq) {
  appId = 0;
  _temp = 0;
  _enc = 0;
  _current = 0;
}

DisplayableManager::~DisplayableManager(void) {
  
}

void DisplayableManager::loadTemp() {
  _current = new Temp(_evSeq);
  _current->bind(EVT_SECOND_TICK, (memberPointer)&Temp::updateReading);
  _current->bind(EVT_BTN_CLICK, (memberPointer)&Temp::toggleScale);
  _temp = (Temp*)_current;
  appId = 1;
}

void DisplayableManager::deleteTemp() {
  if (_temp == 0) {
    return;
  }
  delete _temp;
  _temp = 0;
  _current = 0;
}

void DisplayableManager::loadEncoder() {
  _current = new Encoder(_evSeq);
  _current->bind(EVT_ENCODER, (memberPointer)&Encoder::updateReading);
  _enc = (Encoder*)_current;
  appId = 0;
}

void DisplayableManager::deleteEncoder() {
  if (_enc == 0) {
    return;
  }
  delete _enc;
  _enc = 0;
  _current = 0;
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

  deleteTemp();
  deleteEncoder();

  //deleteCurrent();

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
