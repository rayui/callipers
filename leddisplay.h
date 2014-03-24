/*
  leddisplay.h - library to control 4 digit 7 segment LED
  (c) R Brooks 2014
*/
#ifndef LedDisplay_h
#define LedDisplay_h

#include <Energia.h>

#define NUM_LED_DIGITS 4

#define LED_ON HIGH
#define LED_OFF LOW

#define LED_BRIGHT_OFF 0
#define LED_BRIGHT_LOW 1
#define LED_BRIGHT_MED 40
#define LED_BRIGHT_HI 400

#define SHIFT_ENABLE_OFF HIGH
#define SHIFT_ENABLE_ON LOW

/* numbers 0 - 9

order of segment pins on shift register

C, B, G, D, E, F, A, DP

each arrangement of digit segments is represented by 8 bits - leds a - g and dp
the arrangments that represent the digits 0 - 9 map 1:1 to a number in the range 1 - 256
  
  A
F   B
  G
E   C
  D    DP


The digit zero, for example would be represented as B11011110

*/

class LedDisplay
{
  public:
    LedDisplay();
    void initialize (
      unsigned char digit1Pin,
      unsigned char digit2Pin,
      unsigned char digit3Pin,
      unsigned char digit4Pin,
      unsigned char shiftEnablePin,
      unsigned char latchPin,
      unsigned char shiftClearPin,
      unsigned char shiftClockPin,
      unsigned char shiftDataPin
    );
    void resetLeds();
    void enableDisplay();
    void disableDisplay();
    void strobeInt(int number);
    void strobeFloat(long number, int decimalPlaces);
    void strobeString(char* displayString);
    void setBrightness(int brightness);
    void setDPMask(unsigned char dpMask);
    static unsigned char generateDPMaskFromPosition(int pin);
  private:
    const static unsigned char characterMap[];
    static unsigned char mapSize;
    unsigned char _digitPins[NUM_LED_DIGITS];
    unsigned char _shiftEnablePin;
    unsigned char _latchPin;
    unsigned char _shiftClearPin;
    unsigned char _shiftClockPin;
    unsigned char _shiftDataPin;
    unsigned char _brightness;
    int minimum(int a, int b);
    void strobeSegment(int segment, int mapIndex);
    void strobeFourDigits(int number);
    unsigned char _dpMask;
};

#endif
