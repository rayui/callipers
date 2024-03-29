#include "seven_segment_led.h"
#include "utils.h"

const unsigned char SevenSegmentLED::characterMap[] = {
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

unsigned char SevenSegmentLED::mapSize = sizeof(characterMap) / sizeof(unsigned char);

/*************************************/
/* Constructor
/*************************************/

SevenSegmentLED::SevenSegmentLED() {
  _digitPins[0] = LED_PIN_1;
  _digitPins[1] = LED_PIN_2;
  _digitPins[2] = LED_PIN_3;
  _digitPins[3] = LED_PIN_4;
  _brightness = LED_BRIGHT_HI;
  _strobeRate = LED_STROBE_INTERVAL_MS;
  
  pinMode(_digitPins[0], OUTPUT);
  pinMode(_digitPins[1], OUTPUT);
  pinMode(_digitPins[2], OUTPUT);
  pinMode(_digitPins[3], OUTPUT);
  pinMode(SHIFT_ENABLE_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(SHIFT_CLEAR_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  digitalWrite(SHIFT_ENABLE_PIN, SHIFT_ENABLE_OFF);
  digitalWrite(SHIFT_CLEAR_PIN, HIGH);
  
  _dpMask = 0;

}


/*************************************/
/* Private functions
/*************************************/

int SevenSegmentLED::minimum(int a, int b) {
  if (a <= b) {
   return a;
  }
 
 return b; 
}


void SevenSegmentLED::strobeFourDigits(int number) {
  int i = 0;
  int n = 0;
  
  resetLeds();
  
  enableDisplay();
  
  for (i = 0; i < NUM_LED_DIGITS; i++) {
    n = int(number / Utils::power(10, NUM_LED_DIGITS - i - 1)) % 10;
    strobeSegment(i, n + 7);
  }
  
  disableDisplay();
}

void SevenSegmentLED::strobeSegment(int segment, int mapIndex) {
  int ledPin = _digitPins[segment];
  
  //set decimal point mask
  int decimalPoint = (_dpMask & (1 << (segment))) != 0;
  
  if (mapIndex < 0 || mapIndex > mapSize) {
   mapIndex = 1; //bottom line
  }
  
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, SHIFT_CLOCK_PIN, LSBFIRST, characterMap[mapIndex] ^ (B11111111 ^ decimalPoint));
  digitalWrite(LATCH_PIN, HIGH);
  
  //allow to settle
  
  digitalWrite(ledPin, LED_ON);
  delayMicroseconds(_brightness);
  digitalWrite(ledPin, LED_OFF);
}

/*************************************/
/* Public functions
/*************************************/

void SevenSegmentLED::enableDisplay() {
  digitalWrite(SHIFT_ENABLE_PIN, SHIFT_ENABLE_ON);
}

void SevenSegmentLED::disableDisplay() {
  digitalWrite(SHIFT_ENABLE_PIN, SHIFT_ENABLE_OFF);
}

void SevenSegmentLED::resetLeds() {
  int i = 0;
  int ledPin;

  for (i = 0; i < NUM_LED_DIGITS; i++) {
    ledPin = _digitPins[i];
    digitalWrite(ledPin, LED_OFF);
  } 
}

void SevenSegmentLED::strobeString(char* displayString) { 
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
    } else if (n == 45) {
      n = 2;
    } else if (n = 32) {
      n = 0;
    } else {
      n = 6; //three lines
    }
    
    strobeSegment(i, n);
  }
  
  disableDisplay();
}

void SevenSegmentLED::strobeInt(int number) { 

  //no decimal point
  setDPMask(0);
  
  //restrict to number of digits
  strobeFourDigits(number % int(powf(10, NUM_LED_DIGITS)));
  
}

void SevenSegmentLED::strobeFloat(long number, int decimalPlaces) {
  unsigned char mask = 0;
  
  //set decimal place mask
  mask |= 1 << (NUM_LED_DIGITS - decimalPlaces - 1);
  
  //crop float to integer and show correct number of decimal places  
  setDPMask(mask);
  strobeFourDigits(Utils::cropLongToXDigits(number, NUM_LED_DIGITS));
}

void SevenSegmentLED::setBrightness(int brightness) {
  _brightness = brightness;
}

void SevenSegmentLED::setDPMask(unsigned char dpMask) {
  _dpMask = dpMask; 
}

unsigned char SevenSegmentLED::generateDPMaskFromPosition(int segment) {
  if (segment < 0) {
    return 0;
  }
  return 1 << segment;
}

void SevenSegmentLED::setStrobeRate(int strobeRate) {
  _strobeRate = strobeRate;
}

int SevenSegmentLED::getStrobeRate(void) {
  return _strobeRate;
}
