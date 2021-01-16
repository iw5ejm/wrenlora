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
  EEPROM.get((11 + sizeof(long)), FREQ); //size of long is 4 byte
  PL = EEPROM.read(19);
  
  //read the two aviable payload stored in eeprom
  PLAlen = EEPROM.read(20); //read the lenght of payloadA
  uint8_t L = (uint8_t)PLAlen;
  for (uint8_t i = 0; i < L; i++){
   payloadA[i] = EEPROM.read(21 + i);
    }

  PLBlen = EEPROM.read(130); //read the lenght of payloadA
  L = (uint8_t)PLBlen;
  for (uint8_t i = 0; i < L; i++){
   payloadB[i] = EEPROM.read(131 + i);
    }
  
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
   EEPROM.update(19, PL);
   
   //store the strings for the payload
   EEPROM.update(20,  PLAlen); //store at address 19 the lenght of payload A
   uint8_t L = (uint8_t)PLAlen;
   for (uint8_t i = 0; i < L; i++){
    EEPROM.update(21 + i, payloadA[i]);
     }
  
   EEPROM.update(130,  PLBlen); //store at address 19 the lenght of payload A
   L = (uint8_t)PLBlen;
   for (uint8_t i = 0; i < L; i++){
    EEPROM.update(131 + i, payloadB[i]);
     }
}
