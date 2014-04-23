//displayable

/*
  debouncer.h - library to control 4 digit 7 segment LED
  (c) R Brooks 2014
*/
#ifndef Displayable_h
#define Displayable_h

#include <Energia.h>
#include "event.h"
#include "seven_segment_led.h"

class Displayable : public Eventable {
  public:
    Displayable(EventSequencer* evSeq);
    virtual ~Displayable(void);
    virtual int toString(char* output);
    void setNumDigits(int digits);
    int getNumDigits(int digits);
    virtual unsigned char generateDPMask();
    virtual unsigned char getDPMask();
  protected:
    int _numDigits;
    char _displayString[sizeof(long) * 8 + 1]; //enough to hold a long in binary format
    unsigned char _dpMask;
  private:
};

#endif
