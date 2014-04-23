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
  int dpPosition = 0;
  return generateDPMask(dpPosition);
}

void Encoder::setScale(unsigned char scale) {
  _scale = scale;
}

void Encoder::updateReading(int step) {
  position += step;
  itoa(position, _displayString, 10);
  _displayString[_numDigits] = 0;
}

unsigned char Encoder::generateDPMask(int segment) {
  return dpMask;
}
