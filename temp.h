/*
  temp.h - library to read temperature from msp430
  (c) R Brooks 2014
*/
#ifndef temp_h
#define temp_h

#include <Energia.h>
#include "displayable.h"

#define DEGREES_C 0
#define DEGREES_F 1
#define DEGREES_K 2

#define DEGREES_C_SYMBOL 'C'
#define DEGREES_F_SYMBOL 'F'
#define DEGREES_K_SYMBOL 'K'

#define TEMP_SAMPLE_SIZE 1024  //at a sample rate of 25Hz, this is ~10 seconds of samples

class Temp : public Displayable
{
  public:
    Temp();
    ~Temp();
    void updateReading();
    void initialize(int numDigits, EventSequencer* evSeq);
    void setDegreesC();
    void setDegreesF();
    void setDegreesK();
    int toString(char* output);
  private:
    long runningTotals[3];
    unsigned char _scale;
    long latestReading;
    int getTempXDPs(int scale, int sigFig);
    long voltageToTemp(long voltage, int scale);
    unsigned char setDisplayString(int scale);
    void generateDPMaskFromPosition(int segment);
    void setScale(unsigned char scale);
};

#endif
