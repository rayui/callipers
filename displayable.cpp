#include "displayable.h"

Displayable::Displayable(EventSequencer* evSeq) : Eventable(evSeq) {
  _numDigits = NUM_LED_DIGITS;
}

Displayable::~Displayable(void) {

}

int Displayable::toString(char* output) {
  strncpy(output, _displayString, _numDigits);
  return _numDigits;
}

unsigned char Displayable::generateDPMask() {
  return dpMask;
}

unsigned char Displayable::getDPMask() {
  return dpMask;
}

void Displayable::setNumDigits(int digits) {
  _numDigits = digits;
}

int Displayable::getNumDigits(int digits) {
  return _numDigits; 
}
