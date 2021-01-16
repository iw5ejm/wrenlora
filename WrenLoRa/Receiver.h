//receiver mode
//still to be tested

void Receiver() {
  oled.clear();
  oled.setCursor(15,2);
  oled.println(F("connect USB cable\n         and\n  use serial monitor"));
  Serial.begin(9600);
  while (!Serial);
  delay(500);
  Serial.println(F("\nWrenLoRa Receiver"));
  
  if(RadioInit()) Serial.println(F("LoRa module OK!\n"));
  RadioSet();
  delay(500);

//is the iqi inversion needed?
  Serial.println(F("setting IQI off"));
  Serial2Radio.println(F("radio set iqi off"));
  str = Serial2Radio.readStringUntil('\n');
  Serial.println(str);

  Serial.println(F("setting radio in receive mode"));
  Serial2Radio.println(F("radio rx 0"));
  str = Serial2Radio.readStringUntil('\n');
  Serial.println(str);
  
  while(1) { //enter receiving loop

    while (Serial2Radio.available()) {
      str = Serial2Radio.readStringUntil('\n');
      if ( (str.length() > 1) && (str.indexOf("radio_rx") >= 0) ) { //a packet was received
              Serial.print(F("Received packet '"));
              Serial.print(str);
              Serial.print(F("' RSSI: "));
              Serial2Radio.println(F("radio get rssi"));
              str = Serial2Radio.readStringUntil('\n');
              Serial.print(str);
              Serial.print(F(" SNR: "));
              Serial2Radio.println(F("radio get snr"));
              str = Serial2Radio.readStringUntil('\n');
              Serial.println(str);
                } 
             }
             
    if(Serial.available()) SerialUIedit_settings(); //check if the user sended a command
    }
  
}
