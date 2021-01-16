#include <Arduino.h>

//global variables for LoRaWAN settings, stored in flash
uint8_t  SF;
uint8_t  CR;
uint8_t  CRC;
uint8_t  PWR;
uint16_t BW;
long     FREQ;
uint8_t  ROLE;
uint16_t BCN_DELAY; //interval between transmission in ABP beacon mode
uint16_t NPKT;      //number of packets to send in Packet generator mode 
uint16_t PKT_DELAY; //interval between transmission in Packet generator mode
bool     PL;

char    payloadA[53]; //buffer for the message in the payload
char    payloadB[103]; //buffer for the message in the payload
size_t  PLAlen = 0;
size_t  PLBlen = 0;

#include "Flash.h"
#include "Gui.h"
#include "Radio.h"
#include "Serialui.h"
#include "ABP_beacon.h"
#include "PKT_generator.h"
#include "Receiver.h"


void setup() {
  read_settings(); //read saved settings from eeprom

  //initialize OLED display
  Wire.begin();
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
  oled.set400kHz();
 // oled.setScrollMode(SCROLL_MODE_AUTO); //valutare se necessario

  pinMode(UP_btn,  INPUT_PULLUP);
  pinMode(DWN_btn, INPUT_PULLUP);
  pinMode(SW_btn,  INPUT_PULLUP);

  //start serial communication and ask the user to enter the config menu 
  while (!Serial);
  Serial.begin(9600);
  Serial.println(F("\nWrenLoRa field tester 0.9b"));
  Serial.println(F("send \"e\" within 5 seconds to edit settings"));
  oled.clear(); 
  oled.println(F("\n   Press RED button  \n   within 5 seconds  \n      to enter      \n     CONFIG MENU         \n"));

  while(millis()<5000 && gconf) { //wait for the user to enter the config menu
    if (digitalRead(SW_btn) == BSTATE) GUI();
    if(!(millis()%500)) {oled.print(F("|")); Serial.print(F("."));}
    if(Serial.available()) {Serial.readBytesUntil('\n', aChar, 3); if(aChar[0]=='e') SerialUI();} //check if the user want to modify settings via serial port
    }
  currentSettings(); //print the current settings
  SerialcurrentSettings();
  if(ROLE != 2) Serial.println(F("\n ==! please RESET the board to enter configuration mode again !=="));
  Serial.end();
}

void loop() {

  switch (ROLE) {
    case 1: //PKT GENERATOR
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
