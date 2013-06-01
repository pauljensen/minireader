
#include "Radio.h"

Radio radio = Radio(&Serial1);

float floats[3];
long longs[3];
byte bytes[3];

void setup() {
  Serial.begin(9600);
  while (!radio.is_connected()) {
    Serial.println("waiting for connection...");
    if (radio.wait_for_connection()) {
      Serial.println("no connection received");
    } else {
      Serial.println("CONNECTED");
    }
    delay(1000);
  }
}

void loop() {
  radio.get_bytes(bytes,3);
  radio.send_bytes(bytes,3);
  
  radio.get_longs(longs,3);
  radio.send_longs(longs,3);
  
  radio.get_floats(floats,3);
  radio.send_floats(floats,3);
  
  radio.send_long(radio.get_long());
  radio.send_float(radio.get_float());
}
