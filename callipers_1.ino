#include <Energia.h>
#include "event.h"
#include "debouncer.h"
#include "seven_segment_led.h"
#include "displayable_manager.h"
#include "utils.h"


/* optical encoder pins */

#define ENCODER_PIN_A 5 
#define ENCODER_PIN_B 6

/* control button pin */

#define CTRL_PIN_A 19
#define CTRL_PIN_B 18

static char displayString[NUM_LED_DIGITS + 1];

long volatile encoderStep = 0;

/* event sequencer */

static EventSequencer evSeq;

/* display handler */

static SevenSegmentLED ledDisplay;

/* button monitoring */

static Debouncer debouncerA(&evSeq);
static Debouncer debouncerB(&evSeq);

static DisplayableManager displayManager(&evSeq);

static long clockTick = 0;
static long lastStrobeTick = 0;

static void toggleButtonA() {
  long time = millis();
  debouncerA.checkBounce(time);
}

static void toggleButtonB() {
  long time = millis();
  debouncerB.checkBounce(time);
}

static void handleEncoder() {
  if (digitalRead(ENCODER_PIN_A) == digitalRead(ENCODER_PIN_B)) {
    encoderStep += 1;
  } else {
    encoderStep -= 1;
  }
}

static void setupHardware() {

  analogReference(INTERNAL1V5);
  
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);

  attachInterrupt(ENCODER_PIN_A, &handleEncoder, CHANGE);   //encoder blinked

  pinMode(CTRL_PIN_A, INPUT);
  attachInterrupt(CTRL_PIN_A, &toggleButtonA, FALLING);  //should be on button release

  pinMode(CTRL_PIN_B, INPUT);
  attachInterrupt(CTRL_PIN_B, &toggleButtonB, FALLING);
  
}

void setup()
{

  setupHardware();
  
  displayManager.loadNextApp();
  displayManager.bind(EVT_BTN_DBL_CLICK, (memberPointer)(&DisplayableManager::loadNextApp));
      
  ledDisplay.setBrightness(LED_BRIGHT_HI);
  
}

void loop()
{
  
  clockTick = millis();

  if (clockTick - lastStrobeTick > ledDisplay.getStrobeRate()) {
    
    debouncerA.debounce();
    debouncerB.debounce();
    
    evSeq.consumeEvents();
    
    ledDisplay.setDPMask(displayManager.getDPMask());
    displayManager.toString(displayString);
    ledDisplay.strobeString(displayString);
    
    lastStrobeTick = clockTick;
    debouncerA.trigger(EVT_SECOND_TICK, NULL);

    if (encoderStep != 0) {
      debouncerA.trigger(EVT_ENCODER, (evtArg)&encoderStep);
      encoderStep = 0;
    }
  }
 
}
