
#include "Arduino.h"

#ifndef Radio_h
#define Radio_h

#define WAITING 'W'
#define CONNECT 'C'
#define READY 'R'
#define WANT_TO_TX 'w'
#define READY_TO_RX 'r'
#define ALL_RX 'a'
#define ERROR 'e'
#define TIMEOUT 'T'

class Radio {
   public:
      Radio(HardwareSerial* serial);
      
      int wait_for_connection();
      int connect();
      int is_connected();
      int disconnect();

      void send_byte(byte b);
      byte get_byte();
      int wait_for(byte b);

      void send_bytes(byte* bytes, int n);
      int get_bytes(byte* bytes, int n);

      void send_longs(long* longs, int n);
      int get_longs(long * longs, int n);
      void send_floats(float* floats, int n);
      int get_floats(float* floats, int n);

      void send_long(long l);
      long get_long();
      void send_float(float f);
      float get_float();

   private:
      void _send(byte b);
      int _read_wait;
      int _max_read_waits;
      int _WAITING_delay;
      int _max_connect_tries;
      int _show_debug;
      int _connected;
      HardwareSerial* _serial;
};


#endif

