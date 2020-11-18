#include <EEPROM.h>

void read_settings() {

  SF =   EEPROM.read(0);
  CR =   EEPROM.read(1);
  CRC =  EEPROM.read(2);
  PWR =  EEPROM.read(3);
  ROLE = EEPROM.read(4);
 
  EEPROM.get(5, BCN_DELAY); //uint16_t is 2 byte
  EEPROM.get(7, NPKT);
  EEPROM.get(9, PKT_DELAY);
  
  //read long type variable in eeprom
  EEPROM.get(11, BW);
  EEPROM.get((11 + sizeof(long)), FREQ);
}


void save_settings() {

   EEPROM.update(0,  SF);
   EEPROM.update(1,  CR);
   EEPROM.update(2,  CRC);
   EEPROM.update(3,  PWR);
   EEPROM.update(4,  ROLE);

   EEPROM.put(5, BCN_DELAY); //uint16_t is 2 byte
   EEPROM.put(7, NPKT);
   EEPROM.put(9, PKT_DELAY);

   //store long type variable in eeprom
   EEPROM.put(11, BW);
   EEPROM.put((11 + sizeof(long)), FREQ);
}
