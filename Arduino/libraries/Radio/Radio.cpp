
#include "Arduino.h"
#include "Radio.h"

#define DEBUG(x) if (_show_debug) Serial.println(x);

Radio::Radio(HardwareSerial* serial) {
   _read_wait = 300;  // millis
   _max_read_waits = 10;
   _WAITING_delay = 100;
   _max_connect_tries = 10;
   _show_debug = 1;
   _connected = 0;

   _serial = serial;
   _serial->begin(9600);
}

void Radio::_send(byte b) {
   _serial->write(b);
   _serial->flush();
}

int Radio::wait_for_connection() {
   while (!is_connected()) {
      if (_serial->available() <= 0) {
         _send(WAITING);
         delay(_WAITING_delay);
      } else if (get_byte() == CONNECT) {
         _send(READY);
         _connected = 1;
         break;
      }
   }

   // connection established
   return 0;
}

int Radio::connect() {
   if (is_connected()) {
      return 0;
   }

   for (int i=0; i<_max_connect_tries; i++) {
      if (_serial->available() <= 0) {
         delay(_WAITING_delay);
      } else if (get_byte() == WAITING) {
         _send(CONNECT);
         wait_for(READY);
         _connected = 1;
         return 0;
      }
   }

   // failure to connect
   return 1;
}

int Radio::is_connected() {
   return _connected;
}

int Radio::disconnect() {
   _connected = 0;
   // should tell the other device
   return 0;
}

void Radio::send_byte(byte b) {
   _send(b);
}

byte Radio::get_byte() {
   byte bytes[1];
   for (int i=0; i<_max_read_waits; i++) {
      if (!_serial->available()) {
         delay(_read_wait);
      } else {
         _serial->readBytes((char*) bytes,1);
         return bytes[0];
      }
   }

   // no bytes found
   return TIMEOUT;
}

int Radio::wait_for(byte b) {
   // should implement timeout
   byte rxd = TIMEOUT;
   while (rxd != b) {
      rxd = get_byte();
   }
   return 1;
}

void Radio::send_bytes(byte* bytes, int n) {
   // should warn if n > BUFFER_LENGTH (64?)
   send_byte(WANT_TO_TX);
   wait_for(READY_TO_RX);
   send_byte((byte) n);
   _serial->write(bytes,n);
   _serial->flush();
   byte rcode = get_byte();
   if (rcode == ERROR) {
      Serial.println(">> ERROR");
      // should retransmit here
   }

   if (_show_debug) {
      Serial.print(">> Tx: ");
      for (int i=0; i<n; i++) {
         Serial.print(bytes[i],HEX);
         Serial.print(" ");
      }
      Serial.print(" -- bytes: ");
      Serial.print(n);
      Serial.println();
   }
}

int Radio::get_bytes(byte* bytes, int n) {
   wait_for(WANT_TO_TX);
   send_byte(READY_TO_RX);
   int to_read = (int) get_byte();
   int nread = _serial->readBytes((char*) bytes,n);
   if (nread == to_read) {
      send_byte(ALL_RX);
   } else {
      send_byte(ERROR);
   }

   if (_show_debug) {
      Serial.print(">> Rx: ");
      for (int i=0; i<n; i++) {
         Serial.print(bytes[i],HEX);
         Serial.print(" ");
      }
      Serial.print(" -- bytes: ");
      Serial.print(nread);
      Serial.print(" / ");
      Serial.print(to_read);
      Serial.println();
      Serial.print(">> Available bytes: ");
      Serial.println(_serial->available());
   }
   return nread;
}

void Radio::send_longs(long* longs, int n) {
   byte* bytes = (byte*) longs;
   send_bytes(bytes, 4*n);
}

int Radio::get_longs(long* longs, int n) {
   byte* bytes = (byte*) longs;
   int nread = get_bytes(bytes, 4*n);
   return (int) (nread/4);
}

void Radio::send_floats(float* floats, int n) {
   byte* bytes = (byte*) floats;
   send_bytes(bytes, 4*n);
}

int Radio::get_floats(float* floats, int n) {
   byte* bytes = (byte*) floats;
   int nread = get_bytes(bytes, 4*n);
   return (int) (nread/4);
}

void Radio::send_long(long l) {
   send_longs(&l, 1);
}

long Radio::get_long() {
   long l = 0L;
   get_longs(&l, 1);
   return l;
}

void Radio::send_float(float f) {
   send_floats(&f, 1);
}

float Radio::get_float() {
   float f = 0.0;
   get_floats(&f, 1);
   return f;
}

