#include <Arduino.h>
#include "Wire.h"

#define BIT0 D6
#define BIT1 D7
#define BIT2 D4
#define BIT3 D8
#define Addr 0x48

unsigned int bits = 0;
int del = 30;
unsigned int data[3];
unsigned int debug;

 void iic()
 {
  double raw_adc = (data[0] * 256) + data[1];
  if (raw_adc > 32767)
   {
    raw_adc -= 65536;
   }
  double vadc = (raw_adc / 1237.809f);
  if(bits%2==1)
   {
    vadc = (vadc *(-1));
   }
  
    // Output data to serial monitor
    Serial.print("|");
    Serial.print(bits);
    Serial.print(" ");
    Serial.print(vadc, 4);
    bits++;
    digitalWrite(BIT3, (bits&8)>>3);
    digitalWrite(BIT2, (bits&4)>>2);
    digitalWrite(BIT1, (bits&2)>>1);
    digitalWrite(BIT0, (bits&1));
  }



void setup() {

  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 115200
  Serial.begin(115200);
  Wire.setClock(400000);
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Continuous conversion mode, 8 SPS, 1PGA
  Wire.write(0x01);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(500);


pinMode(BIT0, OUTPUT);
pinMode(BIT1, OUTPUT);
pinMode(BIT2, OUTPUT);
pinMode(BIT3, OUTPUT);

}

void loop() {

delay(del);
Wire.requestFrom(Addr, 2);
if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    
    iic();
  }
 
if(bits > 15)
  {
bits=0;
 Serial.println("");

  } 
}

 