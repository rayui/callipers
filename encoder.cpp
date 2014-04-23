#include "utils.h"
#include "encoder.h"

Encoder::Encoder(EventSequencer* evSeq) : Displayable(evSeq) {
  position = 0;
}

Encoder::~Encoder() {

}

int Encoder::toString(char* output) {
  int numDigits = setDisplayString();
  Displayable::toString(output);
  return numDigits;
}

unsigned char Encoder::setDisplayString() {
  int dpPosition = 2;
  return generateDPMask(dpPosition);
}

void Encoder::setScale(unsigned char scale) {
  _scale = scale;
}

void Encoder::updateReading(long *step) {
  int mmPos = 0;
  position += *step;

  mmPos = position * 254 / 275;

  itoa(mmPos, _displayString, 10);
  _displayString[_numDigits] = 0;
}

unsigned char Encoder::generateDPMask(int segment) {
  if (segment < 0) {
    _dpMask = 0;
  } else {
    _dpMask = 1 << segment;
  }
  
  return _dpMask;
}
