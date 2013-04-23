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
  delayMicroseconds(2500); //tSSRERE_mod3
/*  if (test.poll())
    Serial.println("Comms fuckup");
    
  Serial.print("X: ");
  Serial.println(test.get_x(),DEC);
  Serial.print("Y: ");
  Serial.println(test.get_y(),DEC);*/
  Serial.print("MELEXIS_EE_MAPXYZ: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_MAPXYZ),BIN);
  Serial.print("MELEXIS_EE_3D: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_3D),BIN);
  Serial.print("MELEXIS_EE_FILTER: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_FILTER),BIN);
  Serial.print("MELEXIS_EE_VIRTUALGAINMAX: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_VIRTUALGAINMAX),BIN);
  Serial.print("MELEXIS_EE_VIRTUALGAINMIN: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_VIRTUALGAINMIN),BIN);
  Serial.print("MELEXIS_EE_KALPHA: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_KALPHA),BIN);
  Serial.print("MELEXIS_EE_KBETA: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_KBETA),BIN);
  Serial.print("MELEXIS_EE_SMISM: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_SMISM),BIN);
  Serial.print("MELEXIS_EE_ORTH_B1B2: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_ORTH_B1B2),BIN);
  Serial.print("MELEXIS_EE_KT: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_KT),BIN);
  Serial.print("MELEXIS_EE_PHYST: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_PHYST),BIN);
  Serial.print("MELEXIS_EE_PINFILTER: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_PINFILTER),BIN);
  Serial.print("MELEXIS_EE_USERID_H: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_USERID_H),BIN);
  Serial.print("MELEXIS_EE_USERID_L: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_USERID_L),BIN);

  /*Serial.print("Diag0: ");
  Serial.println(test.get_diag_0(),DEC);
  Serial.print("Diag1: ");
  Serial.println(test.get_diag_1(),DEC);*/
  
}

