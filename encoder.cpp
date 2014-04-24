#include "utils.h"
#include "encoder.h"

Encoder::Encoder(EventSequencer* evSeq) : Displayable(evSeq) {
  position = 0;
  _scale = MILLIMETRES;
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
  int pos = 0;
  int absPos = 0;
  int offset = 0;

  //we multiply position by 254 instead of 25.4 because we want the extra digit of precision
  //compensate by moving decimal place forward

  strncpy(_displayString, "    \0", _numDigits); 

  if (_scale == INCHES) {
    pos = position * 100 / 275;
    dpPosition = 1;
  } else {
    pos = position * 254 / 275;
    if (_scale == CENTIMETRES) {
      dpPosition = 1;
    } else {
      dpPosition = 2;
    }
  }

  absPos = abs(pos);

  //remember, this figure is 10 times greater than actual
  if (absPos < 10) {
    offset = 3;
  } else if (absPos < 100) {
    offset = 2;
  } else if (absPos < 1000) {
    offset = 1;
  }


  if (pos < 0) {
    if (offset > 0) {
      offset -= 1;
    } else {
      pos = absPos;
    }
  }

  itoa(pos, _displayString + offset * sizeof(char), 10);
  _displayString[_numDigits] = 0;

  //fill the string with any necessary zeroes to fake floating point display
  //cover the negative sign if it is there
  if (dpPosition > 0 && offset > dpPosition) {
    int negOffset = (pos < 0);

    for (int i = dpPosition; i < offset + negOffset; i++) {
      _displayString[i] = '0';
    }
  }

  //if fake float is less than 0 but greater than -1, add a leading zero and move the sign
  if (dpPosition > 0 && pos < 0 && offset >= dpPosition) { //i.e. - sign would be on or beyond the decimal point
    _displayString[dpPosition] = '0';
    _displayString[dpPosition - 1] = '-';
  }

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
