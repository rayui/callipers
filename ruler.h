/*
  temp.h - library to read temperature from msp430
  (c) R Brooks 2014
*/
#ifndef temp_h
#define temp_h

#include <Energia.h>
#include "displayable.h"

class Ruler : public Displayable
{
  public:
    Ruler();
    ~Ruler();
    void initialize(int numDigits, EventSequencer* evSeq);
    int toString(char* output);
  private:

};

#endif
