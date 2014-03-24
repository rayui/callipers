#include <Energia.h>
#include "event.h"
#include "debouncer.h"
#include "leddisplay.h"
#include "temp.h"
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

/* event sequencer */

static EventSequencer evSeq;

/* display handler */

static LedDisplay ledDisplay;

/* button monitoring */

static Debouncer debouncerA;
static Debouncer debouncerB;

static int lastButtonEventFlag = 0;
static ButtonEvent lastButtonEvent;

/* temperature sensor */

static Temp tempSensor;

static long clockTick = 0;
static long lastStrobeTick = 0;

static void handleEncoder() {
  if (digitalRead(ENCODER_PIN_B) == LOW) {
    encoderPos--;
  } else {
    encoderPos++;
  }
}

static const void displayTemperature() {  
  char tempString[NUM_LED_DIGITS + 1];
  
  tempSensor.toString(tempString);
  
  ledDisplay.setDPMask(tempSensor.getDPMask());
  ledDisplay.strobeString(tempString);
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
  ledDisplay.initialize(LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4, SHIFT_ENABLE_PIN, LATCH_PIN, SHIFT_CLEAR_PIN, SHIFT_CLOCK_PIN, DATA_PIN);
  debouncerA.initialize(CTRL_PIN_A, &evSeq);
  debouncerB.initialize(CTRL_PIN_B, &evSeq);
  tempSensor.initialize(NUM_LED_DIGITS, &evSeq);
  
  ledDisplay.setBrightness(LED_BRIGHT_HI);
  tempSensor.setDegreesC();

  debouncerA.bind(EVT_BTN_CLICK, &Temp::setDegreesC, &tempSensor);
  debouncerB.bind(EVT_BTN_CLICK, &Temp::setDegreesF, &tempSensor);
  
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

  if (clockTick - lastStrobeTick > LED_STROBE_INTERVAL_MS) {
    
    debouncerA.debounce();
    debouncerB.debounce();
    
    tempSensor.updateReading();
    
    displayTemperature();
    
    evSeq.consumeEvents();
    
    lastStrobeTick = clockTick;

  }
  
}
