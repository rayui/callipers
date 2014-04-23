#include "displayable.h"

Displayable::Displayable(EventSequencer* evSeq) : Eventable(evSeq) {
  _numDigits = NUM_LED_DIGITS;
  strncpy(_displayString, "====\0", _numDigits);
  _dpMask = 0;
}

Displayable::~Displayable(void) {

}

int Displayable::toString(char* output) {
  strncpy(output, _displayString, _numDigits);
  return _numDigits;
}

unsigned char Displayable::generateDPMask() {
  return _dpMask;
}

unsigned char Displayable::getDPMask() {
  return _dpMask;
}

void Displayable::setNumDigits(int digits) {
  _numDigits = digits;
}

int Displayable::getNumDigits(int digits) {
  return _numDigits; 
}
