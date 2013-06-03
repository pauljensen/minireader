
#include "Arduino.h"
#include "SD.h"

#ifndef Reporter_h
#define Reporter_h

// records from a plate reader are stored as a single binary stream:
//
//    bytes  field      format
//    -----  -----      ------
//    0:3    timestamp  long int (two's complement, signed)
//    4      plate id   byte 0x1 - 0x8
//    5:196  OD reads   2 byte unsigned integer in units of milli-ODs (mOD)
//                      (0xFF mOD = 0.255 OD)
//
// For transmission via HTTP or storage in a file, each record is encoded
// as a 394-character string.  Each 2 characters represents a hex encoding
// of a single byte in the record.  All byte patterns are little endian.

#define RECORD_BYTES 197
#define RECORD_CHARS 2 * RECORD_BYTES

class ReporterClass {
   public:
      static byte record_bytes[RECORD_BYTES];
      static char record_chars[RECORD_CHARS];
      static void bytes_to_hex_string(byte* bytes, char* chars, int n);
      static void create_record(long time, byte plate, float* values);

      static int start_SD(int chip_select);
      static void write_SD();
      static void stop_SD();

   private:
      static File SDfile;

};

extern ReporterClass Reporter;

#endif

