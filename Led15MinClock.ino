//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=30;

int standardIntensity=5;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,standardIntensity);
  /* and clear the display */
  lc.clearDisplay(0);
}

/*
 This method will display the characters for the
 word "Arduino" one after the other on the matrix. 
 (you need at least 5x7 leds to see the whole chars)
 */
void diagonalTimer() {
  /* here is the data for the characters */

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
  //byte sixteen[8]   ={B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};

  byte* timeFields[15]={one,two,three,four,five,six,seven,eight,nine,ten,eleven,twelve,thirteen,fourteen,fifteen};
  byte* timeFieldsReverse[15]={fifteen,fourteen,thirteen,twelve,eleven,ten,nine,eight,seven,six,five,four,three,two,one};

  for(byte* timeField : timeFields) {
    for(int i = 0 ; i < 8 ; i++) {
      lc.setRow(0,i,timeField[i]);
    }
    delay(1000);
  }

  /*for(byte* timeField : timeFieldsReverse) {
    for(int i = 0 ; i < 8 ; i++) {
      lc.setRow(0,i,timeField[i]);
    }
    delay(delaytime);
  }*/
}

void heartBeat() {
  byte heart[8]  ={B01100110,B11111111,B11111111,B11111111,B11111111,B01111110,B00111100,B00011000};
  for(int i = 0 ; i < 8 ; i++) {
      lc.setRow(0,i,heart[i]);
  }
  //Outer loop => number of heartbeats
  for (int i = 0 ; i < 4 ; i++) {
    //Raise intensity with loop
    for (int j = 3 ; j <= 15 ; j++) {
      lc.setIntensity(0,j);
      delay(40);  
    }
    //Lower intensity with loop
    for (int k = 15 ; k >= 3 ; k--) {
      lc.setIntensity(0,k);
      delay(40);  
    }
    //Reset to standard led brightness
    lc.setIntensity(0, standardIntensity);
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
    for(int i = 0 ; i < 8 ; i++) {
      lc.setRow(0,i,character[i]);
    }
    delay(1000);
  }
  
}


void displayClock() {
  for (int x = 0 ; x < 3 ; x++) {
    byte a[8]       ={B00111000,B01010100,B10010010,B10010010,B10000010,B01000100,B00111000,B00000000};
    byte b[8]       ={B00111000,B01000100,B10001010,B10010010,B10000010,B01000100,B00111000,B00000000};
    byte c[8]       ={B00111000,B01000100,B10000010,B10011110,B10000010,B01000100,B00111000,B00000000};
    byte d[8]       ={B00111000,B01000100,B10000010,B10010010,B10001010,B01000100,B00111000,B00000000};
    byte e[8]       ={B00111000,B01000100,B10000010,B10010010,B10010010,B01010100,B00111000,B00000000};
    byte f[8]       ={B00111000,B01000100,B10000010,B10010010,B10100010,B01000100,B00111000,B00000000};
    byte g[8]       ={B00111000,B01000100,B10000010,B11110010,B10000010,B01000100,B00111000,B00000000};
    byte h[8]       ={B00111000,B01000100,B10100010,B10010010,B10000010,B01000100,B00111000,B00000000};

    byte* clockPositions[8]={a,b,c,d,e,f,g,h};

    for(byte* clockPosition : clockPositions) {
      for(int i = 0 ; i < 8 ; i++) {
        lc.setRow(0,i,clockPosition[i]);
      }
      delay(500);
    }
  }
}

void loop() { 
  diagonalTimer();
  displayClock();
  displayOperator();
  heartBeat();
}
