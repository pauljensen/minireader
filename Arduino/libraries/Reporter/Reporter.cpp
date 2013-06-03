
#include "Arduino.h"
#include "SD.h"

#include "Reporter.h"

ReporterClass Reporter;
byte ReporterClass::record_bytes[RECORD_BYTES];
char ReporterClass::record_chars[RECORD_CHARS];
File ReporterClass::SDfile;

char hex_digits[] = {'0','1','2','3','4','5','6','7',
                     '8','9','A','B','C','D','E','F'};

void ReporterClass::bytes_to_hex_string(byte* bytes, char* chars, int n) {
   for (int i=0; i<n; i++) {
      chars[2*i] = hex_digits[bytes[i] >> 4];
      chars[2*i+1] = hex_digits[bytes[i] & 0xF];
   }
}

void ReporterClass::create_record(long time, byte plate, float* values) {
   byte* ptr = (byte*) &time;
   for (int i=0; i<4; i++) {
      record_bytes[i] = ptr[i];
   }
   
   record_bytes[4] = plate;
   
   unsigned long mOD = 0L;
   ptr = (byte*) &mOD;
   for (int i=0; i<96; i++) {
      mOD = (unsigned long) (1000L * values[i]);
      for (int j=0; j<2; j++) {
         record_bytes[5+2*i+j] = ptr[j];
      }
   }

   bytes_to_hex_string(record_bytes, record_chars, RECORD_BYTES);
}


// ================ SD card ==================

int ReporterClass::start_SD(int chip_select) {
   // CS is pin 4 on the Ethernet shield
   
   pinMode(10,OUTPUT);
   digitalWrite(10,HIGH); // this is required for the Mega
   pinMode(53,OUTPUT);   // again, for the Mega

   int SD_success = SD.begin(chip_select);
   char filename[] = "data.odb";
   if (SD.exists(filename)) {
      SD.remove(filename);
   }
   SDfile = SD.open(filename, FILE_WRITE);
   return SD_success && SDfile;
}

void ReporterClass::write_SD() {
   SDfile.write((byte*) record_chars, RECORD_CHARS);
   SDfile.println();
   SDfile.flush();
}

void ReporterClass::stop_SD() {
   SDfile.close();
}



