//radio module independent


void PKTgenerator() {
  bool STOP = 1;
  Serial.println(F("\n\rWrenLoRa Packet Generator"));
  if(RadioInit()) Serial.println(F("LoRa module OK!\n"));
  RadioSet();
  
  Serial.print(F("Sending "));
  Serial.print(NPKT);
  Serial.print(F(" packets every "));
  Serial.print(PKT_DELAY);
  Serial.println(F(" ms"));
  
  delay(600);

  while(1){
      Serial.println(F("Send S to start the burst or e to edit settings\n"));
      while(STOP){
        while (Serial.available()) {
            Serial.readBytesUntil('\n', aChar, 3);
            if (aChar[0] == 'S') STOP = 0;
            if (aChar[0] == 'e') {SerialUI(); STOP = 0;}    
          }
      }
    Serial.println("Starting burst...");
    RadioBurst(NPKT,PKT_DELAY);
    delay(500);
    STOP = 1;
    }
}
