#include "leddisplay.h"
#include "utils.h"

unsigned char LedDisplay::characterMap[] = {
  B00000000,  //blank
  B00010000,  //bottom line
  B00100000,  //middle line
  B00000010,  //top line
  B00110000,  //bottom two lines
  B00010010,  //top two lines
  B00110010,  //three lines
  B11011110,  //0
  B11000000,  //1
  B01111010,  //2
  B11110010,  //3
  B11100100,  //4
  B10110110,  //5
  B10111110,  //6
  B11000010,  //7
  B11111110,  //8
  B11110110,  //9
  B11101110,  //A
  B10111100,  //b
  B00111000,  //c
  B11111000,  //d
  B00111110,  //E
  B00101110,  //F
  B10011110,  //G
  B11101100,  //H
  B00001000,  //i
  B11010000,  //J
  B10101100,  //k
  B00011100,  //L
  B11001110,  //M
  B10101000,  //n
  B10111000,  //o
  B01001110,  //P
  B11101100,  //q
  B00101000,  //r
  B10110110,  //S
  B00111100,  //t
  B11011100,  //U
  B10011000,  //v
  B11111100,  //w(ish)
  B01101000,  //x(ish)
  B11110100,  //y
  B01111010   //Z
};

unsigned char LedDisplay::mapSize = sizeof(characterMap) / sizeof(unsigned char);

/*************************************/
/* Constructor
/*************************************/

LedDisplay::LedDisplay(unsigned char digit1Pin, unsigned char digit2Pin, unsigned char digit3Pin, unsigned char digit4Pin, unsigned char shiftEnablePin, unsigned char latchPin, unsigned char shiftClearPin, unsigned char shiftClockPin, unsigned char shiftDataPin) {
 
  _digitPins[0] = digit1Pin;
  _digitPins[1] = digit2Pin;
  _digitPins[2] = digit3Pin;
  _digitPins[3] = digit4Pin;
  _shiftEnablePin = shiftEnablePin;
  _latchPin = latchPin;
  _shiftClearPin = shiftClearPin;
  _shiftClockPin = shiftClockPin;
  _shiftDataPin = shiftDataPin;
  _brightness = LED_BRIGHT_MED;
  
  pinMode(_digitPins[0], OUTPUT);
  pinMode(_digitPins[1], OUTPUT);
  pinMode(_digitPins[2], OUTPUT);
  pinMode(_digitPins[3], OUTPUT);
  pinMode(_shiftEnablePin, OUTPUT);
  pinMode(_latchPin, OUTPUT);
  pinMode(_shiftClearPin, OUTPUT);
  pinMode(_shiftClockPin, OUTPUT);
  pinMode(_shiftDataPin, OUTPUT);

  digitalWrite(_shiftEnablePin, SHIFT_ENABLE_OFF);
  digitalWrite(_shiftClearPin, HIGH);
  
  _dpMask = 0;
}

/*************************************/
/* Private functions
/*************************************/

int LedDisplay::minimum(int a, int b) {
  if (a <= b) {
   return a;
  }
 
 return b; 
}


void LedDisplay::strobeFourDigits(int number) {
  int i = 0;
  int n = 0;
  
  resetLeds();
  
  enableDisplay();
  
  for (i = 0; i < NUM_LED_DIGITS; i++) {
    n = int(number / powf(10, NUM_LED_DIGITS - i - 1)) % 10;
    strobeSegment(i, n + 7);
  }
  
  disableDisplay();
}

void LedDisplay::strobeSegment(int segment, int mapIndex) {
  int ledPin = _digitPins[segment];
  
  //set decimal point mask
  int decimalPoint = (_dpMask & (1 << (segment))) != 0;
  
  if (mapIndex < 0 || mapIndex > mapSize) {
   mapIndex = 1; //bottom line
  }
  
  digitalWrite(_latchPin, LOW);
  shiftOut(_shiftDataPin, _shiftClockPin, LSBFIRST, characterMap[mapIndex] ^ (B11111111 ^ decimalPoint));
  digitalWrite(_latchPin, HIGH);
  
  //allow to settle
  
  digitalWrite(ledPin, LED_ON);
  delayMicroseconds(_brightness);
  digitalWrite(ledPin, LED_OFF);
}

/*************************************/
/* Public functions
/*************************************/

void LedDisplay::enableDisplay() {
  digitalWrite(_shiftEnablePin, SHIFT_ENABLE_ON);
}

void LedDisplay::disableDisplay() {
  digitalWrite(_shiftEnablePin, SHIFT_ENABLE_OFF);
}

void LedDisplay::resetLeds() {
  int i = 0;
  int ledPin;

  for (i = 0; i < NUM_LED_DIGITS; i++) {
    ledPin = _digitPins[i];
    digitalWrite(ledPin, LED_OFF);
  } 
}

void LedDisplay::strobeString(char* displayString) { 
  int i = 0;
  int n = 0;
  int charsToShow = 0;
  unsigned char ledPin = _digitPins[0];
  char croppedString[NUM_LED_DIGITS];
  
  charsToShow = minimum(strlen(displayString), NUM_LED_DIGITS);

  for (int j = 0; j < charsToShow; j++) {
    croppedString[j] = displayString[j];
  }

  resetLeds();
  
  enableDisplay();
  
  for (i = 0; i < charsToShow; i++) {
    n = croppedString[i];
        
    //convert lower case to upper case
    if (n >= 97 && n <= 122) {
      n -= 80;
    //convert upper case to proprietary range
    } else if (n >= 65 && n <= 90) {
      n -= 48;
    //convert numbers to proprietary range
    } else if (n >= 48 && n <= 57) {
      n -= 41;
    //unknown character
    } else {
      n = 6; //three lines
    }
    
    strobeSegment(i, n);
  }
  
  disableDisplay();
}

void LedDisplay::strobeInt(int number) { 

  //no decimal point
  setDPMask(0);
  
  //restrict to number of digits
  strobeFourDigits(number % int(powf(10, NUM_LED_DIGITS)));
  
}

void LedDisplay::strobeFloat(long number, int decimalPlaces) {
  unsigned char mask = 0;
  
  //set decimal place mask
  mask |= 1 << (NUM_LED_DIGITS - decimalPlaces - 1);
  
  //crop float to integer and show correct number of decimal places  
  setDPMask(mask);
  strobeFourDigits(Utils::cropLongToXDigits(number, NUM_LED_DIGITS));
}

void LedDisplay::setBrightness(int brightness) {
  _brightness = brightness;
}

void LedDisplay::setDPMask(unsigned char dpMask) {
  _dpMask = dpMask; 
}

unsigned char LedDisplay::generateDPMaskFromPosition(int segment) {
  if (segment < 0) {
    return 0; 
  }
  return 1 << segment;
}
