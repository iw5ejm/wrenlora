//receiver mode

void Receiver() {
  oled.clear();
  oled.setCursor(15,2);
  oled.println(F("connect USB cable\n         and\n  use serial monitor"));
  Serial.begin(9600);
  while (!Serial);
  Serial.println(F("\nWrenLoRa Receiver"));
  
  if(RadioInit()) Serial.println(F("LoRa module OK!\n"));
  RadioSet();
  
  while(1) { //enter receiving loop
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      // received a packet
      Serial.print("Received packet '");
  
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
    
    if(Serial.available()) SerialUIedit_settings(); //check if the user sended a command
    }
  
}
