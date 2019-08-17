#include <SPI.h>

#include "melexis.h"

MELEXIS test(15);

void setup()
{                
  Serial.begin(38400);  
}

void loop()
{
  delayMicroseconds(2500); //tSSRERE_mod3
  if (test.poll())
    Serial.println("Comms fuckup");
    
  Serial.print("X: ");
  Serial.println(test.get_x(),DEC);
  Serial.print("Y: ");
  Serial.println(test.get_y(),DEC);
  /*Serial.print(" orig: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_3D),BIN);
  Serial.print("write: ");
  Serial.println(test.set_eeprom(MELEXIS_EE_3D,1),DEC);
  Serial.print("  new: ");
  Serial.println(test.get_eeprom(MELEXIS_EE_3D),BIN);

  while (1)
  {
    Serial.print("  done ");
    delay(1000);
  }*/
  /*Serial.print("Diag0: ");
  Serial.println(test.get_diag_0(),DEC);
  Serial.print("Diag1: ");
  Serial.println(test.get_diag_1(),DEC);*/
  
}

