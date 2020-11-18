//Usefull hint for serial communication: https://forum.arduino.cc/index.php?topic=396450.0

void Receiver() {
  Serial.println(F("\n\rWrenLoRa Receiver"));
  
  if(RadioInit()) Serial.println(F("LoRa module OK!\n"));
  RadioSet();

  Serial.println(F("entering receiving loop"));
  while(1) { //enter receiving loop
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      // received a packet
      Serial.print(F("Received packet '"));
  
      // read packet
      while (LoRa.available()) {
        Serial.print((char)LoRa.read());
      }
      // print RSSI of packet
      Serial.print("' RSSI: ");
      Serial.print(LoRa.packetRssi());
      Serial.print(" SNR: ");
      Serial.println(LoRa.packetSnr());
    }
    
    if(Serial.available()) SerialUIedit_settings(); //check if the user has sended a command
    }
  
}
