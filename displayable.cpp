#include "displayable.h"

Displayable::Displayable(void) : Eventable() {

}

Displayable::~Displayable(void) {

}

void Displayable::initialize (int numDigits, EventSequencer* evSeq) {
  Eventable::initialize(evSeq);
  _numDigits = numDigits;
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
