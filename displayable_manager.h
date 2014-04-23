//displayable

/*
  debouncer.h - library to control 4 digit 7 segment LED
  (c) R Brooks 2014
*/
#ifndef Displayable_Manager_h
#define Displayable_Manager_h

#include "event.h"
#include "displayable.h"
#include "temp.h"
#include "encoder.h"

class DisplayableManager : public Eventable {
  public:
    DisplayableManager(EventSequencer* evSeq);
    ~DisplayableManager(void);
    void loadNextApp();
    unsigned char getDPMask();
    int toString(char* output);
  private:
    Displayable* _current;
    int appId;
    void loadTemp();
    void loadEncoder();
    void deleteTemp();
    void deleteEncoder();
    void deleteCurrent();
};

#endif
