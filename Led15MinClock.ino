#include "LedControl.h"

/* pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,1);
int standardIntensity=5;

byte interruptPin = 2;

int countdownStepsInSeconds = 60;

unsigned static int minTimeBetweenInterrupts = 1000;
unsigned static long lastInterrupt = millis();

volatile boolean isCountdownModeActive = false;

void setup() {
  /* for debug only */
  //Serial.begin(9600);
  /* MAX72XX wakeupcall */
  lc.shutdown(0,false);
  lc.setIntensity(0,standardIntensity);
  lc.clearDisplay(0);
  /* Set interupt pin and associated method to call. State = HIGH/LOW/CHANGE */
  attachInterrupt(digitalPinToInterrupt(interruptPin), setAndResetISR, HIGH);
}

void setAndResetISR() {
  /* Make sure that at least minTimeBetweenInterrupts has passed since last
  state change (check is done due to flicker/bouncing) */
  if ((millis() - lastInterrupt) >= minTimeBetweenInterrupts) {
    isCountdownModeActive = !isCountdownModeActive;
    lastInterrupt = millis();
  }
}

void diagonalTimer() {
  
  /* Byte arrays containing data for the diagonal count down pattern */
  byte one[8]       ={B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};
  byte two[8]       ={B11111110,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};
  byte three[8]     ={B11111100,B11111110,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};
  byte four[8]      ={B11111000,B11111100,B11111110,B11111111,B11111111,B11111111,B11111111,B11111111};
  byte five[8]      ={B11110000,B11111000,B11111100,B11111110,B11111111,B11111111,B11111111,B11111111};
  byte six[8]       ={B11100000,B11110000,B11111000,B11111100,B11111110,B11111111,B11111111,B11111111};
  byte seven[8]     ={B11000000,B11100000,B11110000,B11111000,B11111100,B11111110,B11111111,B11111111};
  byte eight[8]     ={B10000000,B11000000,B11100000,B11110000,B11111000,B11111100,B11111110,B11111111};
  byte nine[8]      ={B00000000,B10000000,B11000000,B11100000,B11110000,B11111000,B11111100,B11111110};
  byte ten[8]       ={B00000000,B00000000,B10000000,B11000000,B11100000,B11110000,B11111000,B11111100};
  byte eleven[8]    ={B00000000,B00000000,B00000000,B10000000,B11000000,B11100000,B11110000,B11111000};
  byte twelve[8]    ={B00000000,B00000000,B00000000,B00000000,B10000000,B11000000,B11100000,B11110000};
  byte thirteen[8]  ={B00000000,B00000000,B00000000,B00000000,B00000000,B10000000,B11000000,B11100000};
  byte fourteen[8]  ={B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B10000000,B11000000};
  byte fifteen[8]   ={B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B10000000};
  byte sixteen[8]   ={B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};

  byte* timeFields[15]={one,two,three,four,five,six,seven,eight,nine,ten,eleven,twelve,thirteen,fourteen,fifteen};
  byte* timeFieldsReverse[15]={fifteen,fourteen,thirteen,twelve,eleven,ten,nine,eight,seven,six,five,four,three,two,one};

  for(byte* timeField : timeFields) {
    renderEightRows(timeField);
    if(!delayAndCheckCountdownActiveEverySecond(countdownStepsInSeconds)) {
      return;
    }
  }
  displayClock();
  isCountdownModeActive = false;
}

boolean delayAndCheckCountdownActiveEverySecond(int countdownStepsInSeconds) {
  for (int i = 0 ; i < countdownStepsInSeconds ; i++) {
    delay(1000);
    if (!isCountdownModeActive) {
      return false;
    }  
  }
  return true;
}

boolean delayAndCheckCountDownActiveEverySecondBeginCountDown(int countdownStepsInSeconds) {
  for (int i = 0 ; i < countdownStepsInSeconds ; i++) {
    delay(1000);
    if (isCountdownModeActive) {
      return true;
    }  
  }
  return false;
}

void heartBeat() {
  byte heart[8]  ={B01100110,B11111111,B11111111,B11111111,B11111111,B01111110,B00111100,B00011000};
  renderEightRows(heart);
  /* Outer loop == number of heartbeats */
  for (int i = 0 ; i < 4 ; i++) {
    /* Raise the intensity */
    for (int j = 3 ; j <= 15 ; j++) {
      lc.setIntensity(0,j);
      delay(40);  
    }
    /* Lower the intensity */
    for (int k = 15 ; k >= 3 ; k--) {
      lc.setIntensity(0,k);
      delay(40);  
    }
    lc.setIntensity(0, standardIntensity);
    /* Check if it's time to go to the countdown sequence */
    if (isCountdownModeActive) {
      break;
    }
  }
}

void displayOperator() {
  byte o[8]       ={B00111000,B01000100,B01000100,B01000100,B01000100,B01000100,B01000100,B00111000};
  byte p[8]       ={B01111000,B01000100,B01000100,B01000100,B01111000,B01000000,B01000000,B01000000};
  byte e[8]       ={B01111100,B01000000,B01000000,B01111000,B01000000,B01000000,B01000000,B01111100};
  byte r[8]       ={B01111000,B01000100,B01000100,B01111000,B01001000,B01000100,B01000100,B01000100};
  byte a[8]       ={B00111000,B01000100,B01000100,B01111100,B01000100,B01000100,B01000100,B01000100};
  byte t[8]       ={B01111100,B00010000,B00010000,B00010000,B00010000,B00010000,B00010000,B00010000};
  byte* operatorText[8]={o,p,e,r,a,t,o,r};

  for(byte* character : operatorText) {
    renderEightRows(character);
    delay(1000);
    /* Check if it's time to go to the countdown sequence */
    if (isCountdownModeActive) {
      break;
    }
  }
}


void displayClock() {
  /* Clock arm positions in order for clockwise motion, starting from 12o'clock.
  Utilize as you please */
  byte a[8]       ={B00111000,B01010100,B10010010,B10010010,B10000010,B01000100,B00111000,B00000000};
  byte b[8]       ={B00111000,B01000100,B10001010,B10010010,B10000010,B01000100,B00111000,B00000000};
  byte c[8]       ={B00111000,B01000100,B10000010,B10011110,B10000010,B01000100,B00111000,B00000000};
  byte d[8]       ={B00111000,B01000100,B10000010,B10010010,B10001010,B01000100,B00111000,B00000000};
  byte e[8]       ={B00111000,B01000100,B10000010,B10010010,B10010010,B01010100,B00111000,B00000000};
  byte f[8]       ={B00111000,B01000100,B10000010,B10010010,B10100010,B01000100,B00111000,B00000000};
  byte g[8]       ={B00111000,B01000100,B10000010,B11110010,B10000010,B01000100,B00111000,B00000000};
  byte h[8]       ={B00111000,B01000100,B10100010,B10010010,B10000010,B01000100,B00111000,B00000000};
  byte stopTime[8]={B00111000,B01000100,B10101010,B10010010,B10101010,B01000100,B00111000,B00000000};
  byte* clockPositions[8]={a,h,g,f,e,d,c,b};

  /* Outer for-loop, show the full animation three times */
  for (int x = 0 ; x < 3 ; x++) {
    for(byte* clockPosition : clockPositions) {
      renderEightRows(clockPosition);
      delay(200);
      if (!isCountdownModeActive) {
        break;
      }
    }
  }
  /* When the clock stops spinning, flash the stop icon */
  for(int i = 0 ; i < 20 ; i++) {
    renderEightRows(stopTime);
    delay(300);
    lc.clearDisplay(0);
    delay(300);
    if (!isCountdownModeActive) {
      break;
    }        
  }
}

void renderEightRows(byte rows[]) {
  for(int i = 0 ; i < 8 ; i++) {
        lc.setRow(0,i,rows[i]);
  }
}

void loop() {
  /* Not too nice if ladder to take handle break in e.g. displayOperator so
  that it doesn't go into hearBeat */
  if(isCountdownModeActive) {
    diagonalTimer();
  } 
  if (!isCountdownModeActive){
    displayOperator();
  }
  if (!isCountdownModeActive) {
    heartBeat();  
  }
}
