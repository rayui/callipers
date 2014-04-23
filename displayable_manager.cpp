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
  _temp = new Temp(_evSeq);
  _temp->bind(EVT_SECOND_TICK, (memberPointer)&Temp::updateReading);
  _temp->bind(EVT_BTN_CLICK, (memberPointer)&Temp::toggleScale);
  _current = (Displayable*)_temp;
  appId = 1;
}

void DisplayableManager::deleteTemp() {
  if (_temp == 0) {
    return;
  }
  _temp->unbind(EVT_SECOND_TICK);
  _temp->unbind(EVT_BTN_CLICK);
  delete _temp;
  _temp = 0;
  _current = 0;
}

void DisplayableManager::loadEncoder() {
  _enc = new Encoder(_evSeq);
  _enc->bind(EVT_ENCODER, (memberPointer)&Encoder::updateReading);
  _current = (Displayable*)_enc;
  appId = 0;
}

void DisplayableManager::deleteEncoder() {
  if (_enc == 0) {
    return;
  }
  _enc->unbind(EVT_ENCODER);
  delete _enc;
  _enc = 0;
  _current = 0;
}

void DisplayableManager::loadNextApp() {

  if (_current == 0) {
    loadEncoder();
    return;
  }

  deleteTemp();
  deleteEncoder();

  if (appId == 1) {
    loadEncoder();
  } else {
    loadTemp();
  }

}

unsigned char DisplayableManager::getDPMask() {
  if (_temp != 0) {
    return _temp->getDPMask();    
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
