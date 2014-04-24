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
  int pos = 0;
  int offset = 0;

  //we multiply position by 254 instead of 25.4 because we want the extra digit of precision
  //compensate by moving decimal place forward

  if (_scale == CENTIMETRES) {
    pos = (position * 25.4) / 275;
  } else if (_scale == INCHES) {
    pos = position * 10 / 275; 
  } else {
    pos = position * 254 / 275;
  }

  if (abs(pos) < 10) {
    offset = 3;
  } else if (abs(pos) < 100) {
    offset = 2;
  } else if (abs(pos) < 1000) {
    offset = 1;
  }

  strncpy(_displayString, "    \0", _numDigits); 

  if (pos < 0) {
    if (offset > 0) {
      offset -= 1;
    } else {
      pos = abs(pos);
    }
  }

  itoa(pos, _displayString + offset * sizeof(char), 10);
  _displayString[_numDigits] = 0;

  return generateDPMask(dpPosition);
}

void Encoder::toggleScale() {
  if (_scale == MILLIMETRES) {
    setScale(CENTIMETRES);
  } else if (_scale == CENTIMETRES) {
    setScale(INCHES);
  } else {
    setScale(MILLIMETRES);
  } 
}

void Encoder::setScale(unsigned char scale) {
  _scale = scale;
}

void Encoder::updateReading(long *step) {
  position += *step;
}

unsigned char Encoder::generateDPMask(int segment) {
  if (segment < 0) {
    _dpMask = 0;
  } else {
    _dpMask = 1 << segment;
  }
  
  return _dpMask;
}
