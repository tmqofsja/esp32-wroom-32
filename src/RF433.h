#include <EEPROM.h>

void EEPROM_Write( int , int );
void eeprom_commit();

void BK_Box(int relay,int toggleState[],int RelayPin[],int VPIN_BUTTON[])
{
  toggleState[relay] = !toggleState[relay];
  EEPROM_Write( relay, toggleState[relay]);

  digitalWrite(RelayPin[relay], toggleState[relay]);
  Blynk.virtualWrite(VPIN_BUTTON[relay], toggleState[relay]);
 
  if(!toggleState[relay])
    Serial.println("Device ON");
  else 
    Serial.println("Device OFF");
    delay(100);
}

void EEPROM_Write( int Relay_Pos,int toggle)
  {
    EEPROM.write(Relay_Pos, toggle);
    eeprom_commit();
  }

// Check whether write to EEPROM was successful or not with the EEPROM.commit() function.
void eeprom_commit() {
  
  if (EEPROM.commit()) {
    //Serial.println("EEPROM successfully committed!");
  } else {
    Serial.println("ERROR! EEPROM commit failed!");
  }
} 
