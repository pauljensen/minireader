
#include "SD.h"

#include "Reporter.h"

float values[96];
long time = 10L;
byte plate = 3;

void setup() {
  Serial.begin(9600);
  Serial.println("starting...");
  Serial.flush();
  
  for (int i=0; i<96; i++) {
    values[i] = (float) i/1000.0;
  }
  Reporter.create_record(time,plate,values);
  for (int i=0; i<394; i++) {
    Serial.print(Reporter.record_chars[i]);
    if (i % 2 == 1) Serial.print(" ");
  }
  Serial.println("");
  
  Serial.println("Starting writing...");
  int success = Reporter.start_SD(4);
  Serial.println(success);
  for (int i=0; i<25000; i++) {
    Reporter.write_SD();
    if (i % 500 == 0) Serial.println(i);
  }
  
  Serial.println("finished writing");
  
}

void loop() {}


