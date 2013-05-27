#include <LMP.h>
#include "SPI.h"

const int slaveSelect = 2;

void setup() {
  pinMode(slaveSelect,OUTPUT);
  pinMode(SS,OUTPUT);
  
  Serial.begin(9600);
  while (!Serial) {;}
  Serial.println("testing");
  
  LMP.begin(slaveSelect);
  LMP.quick_start();
  
  Serial.println("finished setup");
  
}

long valuel;
float valuef;
byte data[3];

void loop() {

  valuel = LMP.read_adc_long();
  Serial.println(valuel);
  valuef = LMP.read_adc_float_nonnegative();
  Serial.println(valuef);
  //value = -13;
  //for (int i=0; i<4; i++) {
  //  Serial.println(raw[i],BIN);
  //}
  //Serial.println("");
  
  delay(1000);
}

void show_byte2(byte b) {
  for (int i=0; i<8; i++) {
    if ((b >> 7-i) & 1) {
      break;
    } else {
      Serial.print("0");
    }
  }
  Serial.print(b,BIN);
}

