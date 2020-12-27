//radio module independent code for the 

void PKTgenerator(){
  oled.clear();
  oled.println(F("  PACKET GENERATOR"));
  if(RadioInit()) oled.println(F("LoRa module OK!"));
  RadioSet();
  delay(500);
  
//Request the user the number of packet to send
  oled.print(F("PKT to send? ")); oled.setCursor(94, 2); oled.println(NPKT);
  while(digitalRead(SW_btn) != BSTATE){
    if (digitalRead(UP_btn) == BSTATE) { 
      oled.setCursor(94, 2);
      oled.clearToEOL();
      Iv[8]++;
      if (Iv[8] > 8) Iv[8] = 1;
      NPKT=(PKT_DELAYv[(Iv[8]-1)]/10); //reusing same vector of aviable choice
      oled.println(NPKT);
    }
  
    if (digitalRead(DWN_btn) == BSTATE) { 
      oled.setCursor(94, 2);
      oled.clearToEOL();
      Iv[8]--;
      if (Iv[8] < 1) Iv[8] = 8;
      NPKT=(PKT_DELAYv[(Iv[8]-1)]/10);
      oled.println(NPKT);
    } 
  delay(SDLY); //debounce  
  }  
  
 delay(500); //more debounce  
 
//Request the user the interval between packets
  oled.print(F("interval [ms]? ")); oled.setCursor(94, 3); oled.println(PKT_DELAY);
  while(digitalRead(SW_btn) != BSTATE){
    if (digitalRead(UP_btn) == BSTATE) { 
      oled.setCursor(94, 3);
      oled.clearToEOL();
      Iv[9]++;
      if (Iv[9] > 8) Iv[9] = 1;
      PKT_DELAY=PKT_DELAYv[(Iv[9]-1)];
      oled.println(PKT_DELAY);
    }
  
    if (digitalRead(DWN_btn) == BSTATE) { 
      oled.setCursor(94, 3);
      oled.clearToEOL();
      Iv[9]--;
      if (Iv[9] < 1) Iv[9] = 8;
      PKT_DELAY=PKT_DELAYv[(Iv[9]-1)];
      oled.println(PKT_DELAY);
    } 
  delay(SDLY); //debounce
  }
  
  delay(500); //more debounce 
  
  save_settings(); //eeprom is writted only if settings are changed, to save flash life time :)
  oled.println(F("  press RED button\n      to start"));
  while(digitalRead(SW_btn) != BSTATE) delay(LDLY); //wait for user input to start the burst
  
  RadioBurst(NPKT,PKT_DELAY);
  delay(1500);
  
  //
  gconf=1;
  oled.clear();
  oled.println(F("Press RED button\nto run another burst"));
  oled.println(F("\nPress UP button\nto edit settings"));
  while((digitalRead(SW_btn) != BSTATE)&&gconf) {
    if(digitalRead(UP_btn) == BSTATE) GUI();
    delay(LDLY);
  }
  delay(350);
}
