//displayable

/*
  debouncer.h - library to control 4 digit 7 segment LED
  (c) R Brooks 2014
*/
#ifndef Displayable_Manager_h
#define Displayable_Manager_h

#include <Energia.h>
#include "event.h"
#include "displayable.h"
#include "temp.h"

class DisplayableManager : public Eventable {
  public:
    DisplayableManager(void);
    ~DisplayableManager(void);
    void initialize(EventSequencer* evSeq);
    void loadTemp();
    void deleteTemp();
    void loadNextApp();
    unsigned char getDPMask();
    int toString(char* output);
  private:
    Displayable* _currentApp;
};

#endif
