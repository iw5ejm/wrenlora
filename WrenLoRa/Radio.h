//"driver" for the radio module: configuration and packet bursts transmission

#include <rn2xx3.h>
#include <SoftwareSerial.h>

#define RNrst 11
#define RNtxd 13
#define RNrxd 12
#define TXLED A3

//useful variables for at commands
String str;
char sentence[150];
char band = "L";
const char radioset[] = "radio set "; 
const char radiotx[] = "radio tx ";
String hweui;


//create an instance of the rn2xx3 library,
//giving the software serial as port to use to connect the radio module
SoftwareSerial Serial2Radio(RNtxd, RNrxd); // RX (connect to TX of other device), TX (connect to RX of other device)
rn2xx3 myLora(Serial2Radio);

bool RadioInit() {
  Serial.begin(9600);
  Serial2Radio.begin(9600);
  pinMode(TXLED, OUTPUT);
  pinMode(RNrst, OUTPUT);
  digitalWrite(RNrst, LOW);
  delay(500);
  digitalWrite(RNrst, HIGH);
  delay(100); //wait for the RN2483's startup message
  Serial2Radio.flush();
  //Autobaud the RN2483 module to 9600 bps. The default would otherwise be 57600 and such a rate is to much for software serial
  myLora.autobaud();

  //check connection with RN2483 module
  hweui = myLora.hweui();
  while(hweui.length() != 16)
  { 
    oled.clear();
    oled.println(F("Starting LoRa \nfailed!"));
    Serial.println(F("Starting LoRa module failed!"));
    delay(10000);
    hweui = myLora.hweui();
    
  }
  Serial.print(F("HWeui is:")); Serial.println(myLora.hweui());
  return 1;
}


/*
 * This function blocks until the word "radio_tx_ok\n" is received on the UART,
 * used in the burst: the next packet isn't send until the previous one has expired its on air time
 */
int wait_for_txok()
{

  
  str = Serial2Radio.readStringUntil('\n');
  if ( str.indexOf("radio_tx_ok") == -1 ) {
    Serial.println(str);
    return 1;
    }
    
  else {
    Serial.println(str);
    return 0;
  }
 

}

void RadioSet() {
  //block upper layers of LoRaWAN protocol to edit settings
  Serial.println(F("pausing mac layer"));
  Serial2Radio.println(F("mac pause"));
  str = Serial2Radio.readStringUntil('\n');
  Serial.println(str);
  
  Serial2Radio.println("radio set mod lora");
  str = Serial2Radio.readStringUntil('\n');
  Serial.println(str);
  
  Serial.println(F("\nsetting freq"));
  sprintf(sentence, "%sfreq %6ld",radioset,FREQ);
  Serial.println(sentence);
  Serial2Radio.println(sentence);
  str = Serial2Radio.readStringUntil('\n');
  Serial.println(str);

  Serial.println(F("\nsetting SF"));
  sprintf(sentence, "%ssf sf%d",radioset,SF);
  Serial.println(sentence);
  Serial2Radio.println(sentence);
  str = Serial2Radio.readStringUntil('\n');
  Serial.println(str);
  
  Serial.println(F("\nsetting CR"));
  sprintf(sentence, "%scr 4/%d",radioset,CR);
  Serial.println(sentence);
  Serial2Radio.println(sentence);
  str = Serial2Radio.readStringUntil('\n');
  Serial.println(str);
  
  Serial.println(F("\nsetting rxbw"));
  sprintf(sentence, "%srxbw %d",radioset,BW);
  Serial.println(sentence);
  Serial2Radio.println(sentence);
  str = Serial2Radio.readStringUntil('\n');
  Serial.println(str);

  Serial.println(F("\nsetting bw"));
  sprintf(sentence, "%sbw %d",radioset,BW);
  Serial.println(sentence);
  Serial2Radio.println(sentence);
  str = Serial2Radio.readStringUntil('\n');
  Serial.println(str);

  Serial.println(F("\nsetting PWR"));
  sprintf(sentence, "%spwr %d",radioset,PWR);
  Serial.println(sentence);
  Serial2Radio.println(sentence);
  str = Serial2Radio.readStringUntil('\n');
  Serial.println(str);
  
  // Serial2Radio.println("radio set sync 34"); //or 12?
  Serial.println(F("\nsetting CRC"));
  if(CRC) Serial2Radio.println(F("radio set crc on")); else Serial2Radio.println(F("radio set crc off"));
  str = Serial2Radio.readStringUntil('\n');
  Serial.println(str); 
  
  }




void RadioBurst(uint16_t npkt, uint16_t ms) {
  
  uint8_t counter = npkt;
  
  oled.clear(15,128,7,7);
  oled.println(F("WALK AWAY! :)"));
  delay(3000); //give the time to the operator to get away from WrenLoRa


  //block upper layers of LoRaWAN protocol
  Serial2Radio.println(F("mac pause"));

//send a packet with a summary of current settings
  Serial.println(F("\nsend a packet"));
  //check if we are using low or high band and generate the summary packet
  if (FREQ<438575000L) sprintf(sentence, "bursting 433@SF%d BW%d CR4/%d",SF,BW,CR); else sprintf(sentence, "bursting 868@SF%d BW%d CR4/%d",SF,BW,CR);
  Serial.print(F("raw sentence: ")); Serial.println(sentence); 
  Serial2Radio.print(F("radio tx ")); Serial2Radio.println(myLora.base16encode(sentence));
 
  //wait for summary packet to be sent
  digitalWrite(TXLED, HIGH);
  while(wait_for_txok());
  digitalWrite(TXLED, LOW);
  
  delay(500);
  
  //enter the burst loop
  while(counter){

    if(PL) sprintf(sentence, "%s %3d", payloadA, counter); else sprintf(sentence, "%s %3d", payloadB, counter); //check if we are using payload A or B and produce the sentence to be encoded
    Serial2Radio.print(F("radio tx ")); Serial2Radio.println(myLora.base16encode(sentence));
    digitalWrite(TXLED, HIGH);
    while(wait_for_txok());
    digitalWrite(TXLED, LOW);
    
    counter--;

    oled.clear(15,128,7,7);
    oled.print(counter); oled.println(F(" remaining PKT"));

    delay(ms);
  }
  
}
