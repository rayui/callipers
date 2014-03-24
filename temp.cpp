/*

analogReference(INTERNAL1V5);
A = (uint32_t)analogRead(TEMPSENSOR);

C = (A * 27069 - 18169625) >> 16
K = (A * 27069 - 268467) >> 16
F = (A * 48724 - 30634388) >> 16

*/

#include "utils.h"
#include "temp.h"

Temp::Temp() : Displayable() {

}

Temp::~Temp() {

}

void Temp::initialize(int numDigits, EventSequencer* evSeq) {
  Displayable::initialize(numDigits, evSeq);
  _scale = DEGREES_C;
  latestReading = 0;
  analogReference(INTERNAL1V5);
  runningTotals[DEGREES_C] = 0;
  runningTotals[DEGREES_F] = 0;
  runningTotals[DEGREES_K] = 0;
  
  //disregard first few readings
  for (int i = 10; i--; i > 0) {
    latestReading = (long)analogRead(TEMPSENSOR);
  }
  
  for (int i = TEMP_SAMPLE_SIZE; i--; i > 0) {
    latestReading = (long)analogRead(TEMPSENSOR);
    for (int j = 3; j--; j > 0) {
      runningTotals[j] += voltageToTemp(latestReading, j);
    }
    delay(1);
  }
}

int Temp::toString(char* output) {
  int numDigits = setDisplayString(_scale);
  Displayable::toString(output);
  return numDigits;
}

void Temp::setDegreesC() {
  setScale(DEGREES_C);
}

void Temp::setDegreesF() {
  setScale(DEGREES_F);  
}

void Temp::setDegreesK() {
  setScale(DEGREES_K);
}

//see here for the algorithm:
//http://forum.43oh.com/topic/1954-using-the-internal-temperature-sensor/#entry16834

long Temp::voltageToTemp(long voltage, int scale) {
  long temp = 0;
  
  if (scale == DEGREES_F) {
    temp = (voltage * 48724 - 30634388) >> 16;
  } else if (scale == DEGREES_K) {
    temp = (voltage * 27069 - 268467) >> 16;    
  } else {
    //default to degrees C 
    temp = (voltage * 27069 - 18169625) >> 16;
  }
  
  return temp;
}

int Temp::getTempXDPs(int scale, int numDPs) {
  long multipler = Utils::power(10, numDPs);
  long temp = (multipler * runningTotals[scale]) >> 10;  //1024 = 2^10
  return temp;
}

unsigned char Temp::setDisplayString(int scale) {

  //we can think about numDigits as number of significant figures
  //we are always going to have 2 decimal places, some of which will eventually be cropped off to fit on the display
  //the last two digits will always be decimal places because we store floats as ints
  
  //need 17 bytes to store a full int
  int i = 0;
  char scaleSymbol = DEGREES_C_SYMBOL;
  int temp = getTempXDPs(scale, 2);
  int dpPosition = -1;
  
  if (_scale == DEGREES_K) {
    scaleSymbol = DEGREES_K_SYMBOL;
  } else if (_scale == DEGREES_F) {
    scaleSymbol = DEGREES_F_SYMBOL;
  }
  
  //we turn it into a string and find the end
  itoa(temp, _displayString, 10);
  while(_displayString[i] != 0) {
    i++;
  }
  
  if (i > 2 && scaleSymbol != DEGREES_K_SYMBOL) {
    dpPosition = i - 3;
  }
  
  //and replace the last character with the symbol
  _displayString[_numDigits - 1] = scaleSymbol;
  _displayString[_numDigits] = 0;
  
  //we return the position of the decimal point
  //if it's < 0 there is no decimal point
  generateDPMaskFromPosition(dpPosition);
  
  return dpMask;
}

void Temp::updateReading() {
  
  latestReading = (long)analogRead(TEMPSENSOR);  //get latest reading

  for (int j = 3; j--; j > 0) {
    runningTotals[j] -= (runningTotals[j] >> 10);  //subtract average from running total. 1024 = 2^10
    runningTotals[j] += voltageToTemp(latestReading, j);  //add latest reading
  }
  
}

void Temp::generateDPMaskFromPosition(int segment) {
  if (segment < 0) {
    dpMask = 0;
  } else {
    dpMask = 1 << segment;
  }
}

void Temp::setScale(unsigned char scale) {
  _scale = scale;
}