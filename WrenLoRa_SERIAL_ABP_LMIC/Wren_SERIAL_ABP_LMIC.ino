#include <Arduino.h>

//global variables for settings, stored in flash
uint8_t  SF;
uint8_t  CR;
uint8_t  CRC;
uint8_t  PWR;
long     BW;
long     FREQ;
uint8_t  ROLE;
uint16_t BCN_DELAY; //interval between transmission in ABP beacon mode
uint16_t NPKT;      //number of packets to send in Packet generator mode 
uint16_t PKT_DELAY; //interval between transmission in Packet generator mode

#include "Flash.h"
#include "Variables.h"
#include "Radio.h"
#include "Serialui.h"
#include "ABP_beacon.h"
#include "PKT_generator.h"
#include "Receiver.h"


void setup() {
  read_settings(); //read saved settings from eeprom
    
  while (!Serial);
  Serial.begin(9600);
  Serial.println(F("\nWrenLoRa field tester 0.9b"));
  Serial.println(F("send \"e\" within 5 seconds to edit settings"));

  while(millis()<5000 && sconf) { //wait for the user to enter the config menu
    if(!(millis()%1000)) {Serial.print(F("."));}
    if(Serial.available()) {Serial.readBytesUntil('\n', aChar, 3); if(aChar[0]=='e') SerialUI();} //check if the user want to modify settings via serial port
    }
  SerialcurrentSettings();
}

void loop() {

  switch (ROLE) {
    case 1:
    PKTgenerator();
    break;
    
    case 2: //RECEIVER
    Receiver();
    break;
    
    case 3: //ABP BEACON
    ABPbeacon();
    break;
    
  } 
}
