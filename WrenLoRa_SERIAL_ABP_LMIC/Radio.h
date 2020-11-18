#include <SPI.h>
#include <LoRa.h>

#define UHFNSSpin 10
#define SHFNSSpin 8
#define UHFNSS HIGH
#define SHFNSS HIGH


bool RadioInit() {
//FREQ<<500000000L
  if (FREQ<438575000L) {
    //We have to use the Ra-02 433 MHz module
    Serial.println(F("\n\r433 MHz Ra-02 selected"));
    pinMode(SHFNSSpin,OUTPUT);
    digitalWrite(SHFNSSpin, SHFNSS); //Disable 868 MHz module
    pinMode(7,INPUT); // RST
    pinMode(3,INPUT); //DIO0
    pinMode(5,INPUT); //DIO1
    pinMode(6,INPUT); //DIO2
    //LoRa.setPins(ss, reset, dio0)
    //dio0 must be interrupt capable via Arduinos attachInterrupt
    LoRa.setPins(UHFNSSpin, 9, 2);    
  }
  else {
    //We have to use the RFM95W 868 MHz module
    Serial.println(F("\n\r868 MHz RFM95w selected"));
    pinMode(UHFNSSpin,OUTPUT);
    digitalWrite(UHFNSSpin, UHFNSS); //Disable 433 MHz module
    pinMode(9,INPUT); // RST
    pinMode(2,INPUT); //DIO0
    pinMode(5,INPUT); //DIO1
    pinMode(6,INPUT); //DIO2
    //LoRa.setPins(ss, reset, dio0)
    //dio0 must be interrupt capable via Arduinos attachInterrupt
    LoRa.setPins(SHFNSSpin, 7, 3); 
  }
  
  if (!LoRa.begin(FREQ)) {
    Serial.println(F("\n\rStarting LoRa failed!"));
    while (1);
  }
  return 1;
  
}



void RadioSet() {
 
  LoRa.setSpreadingFactor(SF);
  LoRa.setCodingRate4(CR); //è il denominatore del CR: aviable choices
  LoRa.setSignalBandwidth(BW); //es BW = 125E3 for 125 kHz
  LoRa.setSyncWord(0x34);
  if(CRC) LoRa.enableCrc(); else LoRa.disableCrc();
  LoRa.setTxPower(PWR);
  
  }




void RadioBurst(uint16_t npkt, uint16_t ms) {
  
  uint8_t counter = npkt;

  LoRa.beginPacket();
  LoRa.print(F("Sending "));
  LoRa.print(counter);
  LoRa.print(F(" PKTS @SF "));
  LoRa.print(SF);
  LoRa.endPacket(); //close the packet and send it
  
  while(counter){
  
    // send packet
    LoRa.beginPacket();
    LoRa.print(F("hello "));
    LoRa.print(counter);
    LoRa.endPacket(); //close the packet and send it
  
    counter--;

    Serial.print(counter); Serial.println(" remaining PKT");

    delay(ms);
  }
  
}
