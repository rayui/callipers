#include <Energia.h>
#include "event.h"
#include "debouncer.h"
#include "seven_segment_led.h"
#include "displayable_manager.h"
#include "utils.h"

#define LED_STROBE_INTERVAL_MS 20 //milliseconds

/* shift register pins */

#define LED_PIN_1 15
#define LED_PIN_2 14
#define LED_PIN_3 13
#define LED_PIN_4 12
#define SHIFT_ENABLE_PIN 7
#define LATCH_PIN 8
#define SHIFT_CLEAR_PIN 10
#define SHIFT_CLOCK_PIN 9
#define DATA_PIN 11

/* optical encoder pins */

#define ENCODER_PIN_A 5
#define ENCODER_PIN_B 6

/* control button pin */

#define CTRL_PIN_A 19
#define CTRL_PIN_B 18

static long encoderPos = 0;
static int encoderPosLast = LOW;

char displayString[NUM_LED_DIGITS + 1];

/* event sequencer */

static EventSequencer evSeq;

/* display handler */

static SevenSegmentLED ledDisplay;

/* button monitoring */

static Debouncer debouncerA;
static Debouncer debouncerB;

static int lastButtonEventFlag = 0;
static volatile ButtonEvent lastButtonEvent;

static DisplayableManager displayManager;

static long clockTick = 0;
static long lastStrobeTick = 0;

static void handleEncoder() {
  if (digitalRead(ENCODER_PIN_B) == LOW) {
    encoderPos--;
  } else {
    encoderPos++;
  }
}

static void toggleButtonA() {
  lastButtonEvent.pin = CTRL_PIN_A;
  lastButtonEvent.time = millis();
  lastButtonEventFlag++;
}

static void toggleButtonB() {
  lastButtonEvent.pin = CTRL_PIN_B;
  lastButtonEvent.time = millis();
  lastButtonEventFlag++;
}


static void setupHardware() {
  
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);

  attachInterrupt(ENCODER_PIN_A, handleEncoder, RISING);   //encoder blinked

  pinMode(CTRL_PIN_A, INPUT);
  attachInterrupt(CTRL_PIN_A, &toggleButtonA, FALLING);  //should be on button release

  pinMode(CTRL_PIN_B, INPUT);
  attachInterrupt(CTRL_PIN_B, &toggleButtonB, FALLING);
  
  
}

void setup()
{
  
  lastButtonEventFlag = 0;
  setupHardware();
  
  evSeq.initialize();
  ledDisplay.initialize(LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4, SHIFT_ENABLE_PIN, LATCH_PIN, SHIFT_CLEAR_PIN, SHIFT_CLOCK_PIN, DATA_PIN, LED_STROBE_INTERVAL_MS);
  debouncerA.initialize(CTRL_PIN_A, &evSeq);
  debouncerB.initialize(CTRL_PIN_B, &evSeq);
  displayManager.initialize(&evSeq);
  
  //the following code contains a memroy leak
  
  displayManager.loadTemp();
  displayManager.bind(EVT_BTN_DBL_CLICK, &DisplayableManager::loadNextApp);
      
  ledDisplay.setBrightness(LED_BRIGHT_HI);
  

  
}

void loop()
{
  
  if (lastButtonEventFlag != 0) {
    if (lastButtonEvent.pin == CTRL_PIN_A) {
      debouncerA.addEvent(&lastButtonEvent);      
    } else {
      debouncerB.addEvent(&lastButtonEvent);
    }

    lastButtonEventFlag = 0;
  }
  
  clockTick = millis();

  if (clockTick - lastStrobeTick > ledDisplay.getStrobeRate()) {
    
    debouncerA.debounce();
    debouncerB.debounce();
    
    evSeq.consumeEvents();
    
    displayManager.toString(displayString);
    ledDisplay.setDPMask(displayManager.getDPMask());
    ledDisplay.strobeString(displayString);
//    ledDisplay.strobeInt(evSeq.getNumSubscriptions());
    
    lastStrobeTick = clockTick;
    debouncerA.trigger(EVT_SECOND_TICK);

  }
  
}
