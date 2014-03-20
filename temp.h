/*
  temp.h - library to read temperature from msp430
  (c) R Brooks 2014
*/
#ifndef temp_h
#define temp_h

#include <Energia.h>

#define DEGREES_C 0
#define DEGREES_F 1
#define DEGREES_K 2

#define DEGREES_C_SYMBOL 'C'
#define DEGREES_F_SYMBOL 'F'
#define DEGREES_K_SYMBOL 'K'

#define TEMP_SAMPLE_SIZE 256  //at a sample rate of 25Hz, this is ~10 seconds of samples

template <class Base>
class Temp : public Base
{
  public:
    Temp(EventSequencer* evSeq);
    int getTempXDPs(int scale, int sigFig);
    int getTempString(int scale, char *output, int numDigits);
    void updateReading();
    void initialize();
    void printTemp();
  private:
    long runningTotals[3];
    long latestReading;
    long voltageToTemp(long voltage, int scale);
    unsigned char dpMask;
};

#include "temp_impl.h"

#endif
