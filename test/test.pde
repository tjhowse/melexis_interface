#include <SPI.h>

/* Serial Monitor Example, Teensyduino Tutorial #3
   http://www.pjrc.com/teensy/tutorial3.html

   After uploading this to your board, use Serial Monitor
   to view the message.  When Serial is selected from the
   Tools > USB Type menu, the correct serial port must be
   selected from the Tools > Serial Port AFTER Teensy is
   running this code.  Teensy only becomes a serial device
   while this code is running!  For non-Serial types,
   the Serial port is emulated, so no port needs to be
   selected.

   This example code is in the public domain.
*/
#include "D:\devstuff\melexis_interface\melexis.cpp"

MELEXIS test;

void setup()
{                
  Serial.begin(38400);  
}

void loop()
{
  delay(1); //tSSRERE_mod3
  if (test.poll())
    Serial.println("Comms fuckup");

  Serial.print("X: ");
  Serial.println(test.get_x(),DEC);
  Serial.print("Y: ");
  Serial.println(test.get_y(),DEC);
  Serial.print("Z: ");
  Serial.println(test.get_z(),DEC);
  /*Serial.print("Diag0: ");
  Serial.println(test.get_diag_0(),DEC);
  Serial.print("Diag1: ");
  Serial.println(test.get_diag_1(),DEC);*/
  Serial.print("Roll: ");
  Serial.println(test.get_roll(),DEC);
}

