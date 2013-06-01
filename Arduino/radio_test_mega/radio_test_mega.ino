
#include "Radio.h"

Radio radio = Radio(&Serial1);

float floats[] = {1.2, -2.3, 3.4};
long longs[] = {-10L, 11L, -12L};
byte bytes[] = {0xF, 0x1, 0x0};

void setup() {
  Serial.begin(9600);
  while (!radio.is_connected()) {
    Serial.println("trying to establish connection");
    if (radio.connect()) {
      Serial.println("no connection established.");
    } else {
      Serial.println("CONNECTED");
    }
    delay(1000);
  }
}

void loop() {
  radio.send_bytes(bytes,3);
  radio.get_bytes(bytes,3);
  for (int i=0; i<3; i++) {
    Serial.print(bytes[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
  
  radio.send_longs(longs,3);
  radio.get_longs(longs,3);
  for (int i=0; i<3; i++) {
    Serial.print(longs[i]);
    Serial.print(" ");
  }
  Serial.println("");
  
  radio.send_floats(floats,3);
  radio.get_floats(floats,3);
  for (int i=0; i<3; i++) {
    Serial.print(floats[i]);
    Serial.print(" ");
  }
  Serial.println("");
  
  radio.send_long(121L);
  Serial.println(radio.get_long());
  
  radio.send_float(3.14159);
  Serial.println(radio.get_float());
  
  delay(1000);
}

