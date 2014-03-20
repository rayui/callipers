#include "event.h"
#include "debouncer.h"
#include "leddisplay.h"
//#include "menu.h"
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

#define CTRL_PIN_A 5
#define CTRL_PIN_B 18

#define DISPLAY_MODE_DEFAULT  0
#define DISPLAY_MODE_MENU  1

#define NUM_INTERRUPT_EVENTS 5

long encoderPos = 0;
int encoderPosLast = LOW;

unsigned char displayMode = DISPLAY_MODE_DEFAULT;

/* event sequencer */

EventSequencer* evSeq = 0;

/* display handler */

LedDisplay* display = 0;

/* button monitoring */

Debouncer<Eventable>* debouncerA = 0;
Debouncer<Eventable>* debouncerB = 0;

int currentPinVal = 0;
int lastButtonEventFlag = 0;
ButtonEvent* lastButtonEvent = 0;

/* temperature sensor */

Temp<Eventable>* tempSensor = 0;

/* menu */

//Menu* menu = 0;

long clockTick = 0;
long lastStrobeTick = 0;

void handleEncoder() {
  if (digitalRead(ENCODER_PIN_B) == LOW) {
    encoderPos--;
  } else {
    encoderPos++;
  }
}

void displayTemperature() {  
  int dpPosition;
  char tempString[NUM_LED_DIGITS + 1];
  unsigned char scale;
  
  if (clockTick % 15000 > 10000) {
    scale = DEGREES_K;
  } else if (clockTick % 15000 > 5000) {
    scale = DEGREES_F;
  } else {
    scale = DEGREES_C;
  }
  
  dpPosition = tempSensor->getTempString(scale, tempString, NUM_LED_DIGITS);
  
  display->setDPMask(LedDisplay::generateDPMaskFromPosition(dpPosition));

  display->strobeString(tempString);
}

void toggleButtonA() {
  //initialized to rising
  if (digitalRead(CTRL_PIN_A) != CTRL_PIN_RELEASE) {
    return;
  }
  lastButtonEvent->pin = CTRL_PIN_A;
  lastButtonEvent->time = millis();
  lastButtonEventFlag++;
}

void toggleButtonB() {
  //initialized to rising
  if (digitalRead(CTRL_PIN_B) != CTRL_PIN_RELEASE) {
    return;
  }
  lastButtonEvent->pin = CTRL_PIN_B;
  lastButtonEvent->time = millis();
  lastButtonEventFlag++;
}

void click() {
  Serial.println("CLICK"); 
}

void doubleClick() {
  Serial.println("DOUBLE CLICK"); 
}


void setupHardware() {
/*  display = new LedDisplay(LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4, SHIFT_ENABLE_PIN, LATCH_PIN, SHIFT_CLEAR_PIN, SHIFT_CLOCK_PIN, DATA_PIN);
  display->setBrightness(LED_BRIGHT_HI);*/
  
  tempSensor = new Temp<Eventable>(evSeq);
  tempSensor->initialize();
  
/*  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);

  attachInterrupt(ENCODER_PIN_A, handleEncoder, RISING);   //encoder blinked*/

  pinMode(CTRL_PIN_A, INPUT_PULLUP);
  attachInterrupt(CTRL_PIN_A, &toggleButtonA, RISING);  //should be on button release

  pinMode(CTRL_PIN_B, INPUT_PULLUP);
  attachInterrupt(CTRL_PIN_B, &toggleButtonB, CHANGE);
  
  
}

void setup()
{
  evSeq = new EventSequencer();
  debouncerA = new Debouncer<Eventable>(CTRL_PIN_A, evSeq);
  debouncerB = new Debouncer<Eventable>(CTRL_PIN_B, evSeq);
  
  Serial.begin(4800); 
  
  Serial << "STARTING HERE\n\n\n\n\n";

  setupHardware();

/*  menu = new Menu;
  menu->initialize();*/

  lastButtonEvent = new ButtonEvent;
  lastButtonEventFlag = 0;

  debouncerA->bind(EVT_BTN_CLICK, &Temp<Eventable>::printTemp, tempSensor);
  debouncerA->bind(EVT_BTN_DBL_CLICK, &Debouncer<Eventable>::onDoubleClick, debouncerA);
  
  Serial.println("done setup\n\n\n");

}

void loop()
{
  
  if (lastButtonEventFlag != 0) {
    debouncerA->addEvent(lastButtonEvent);
    lastButtonEventFlag = 0;
  }
  
  clockTick = millis();

  if (clockTick - lastStrobeTick > LED_STROBE_INTERVAL_MS) {
    
    debouncerA->debounce();
    
    tempSensor->updateReading();
    
    evSeq->consumeEvents();
    
    lastStrobeTick = clockTick;

  }
  
}
