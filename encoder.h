/*
  encoder.h - library to read monopole encoder from msp430
  (c) R Brooks 2014
*/
#ifndef encoder_h
#define encoder_h

#include <Energia.h>
#include "displayable.h"

#define MILLIMETRES 0
#define INCHES 1

#define MILLIMETRES_SYMBOL "mm"
#define INCHES_SYMBOL "inch"

class Encoder : public Displayable
{
  public:
    Encoder(EventSequencer* evSeq);
    virtual ~Encoder();
    void updateReading(int encoderPinBState);
    virtual unsigned char generateDPMask(int segment);
    virtual int toString(char* output);
    void toggleScale();
  private:
    int position;
    unsigned char _scale;
    long latestReading;
    int getXDPs(int scale, int sigFig);
    long clicksToLength(long clicks, int scale);
    unsigned char setDisplayString();
    void setScale(unsigned char scale);
};

#endif

