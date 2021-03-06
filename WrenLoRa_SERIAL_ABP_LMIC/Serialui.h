bool    sconf = 1; //check if we are in configuration mode to edit the parameters
char    aChar[4]; //incoming serial buffer

void aviableCommands(){
  Serial.println(F("\n\r= Aviable commands: ="));
  Serial.println(F("h print this menù\n\rH sHow current settings\n\rS save settings and quit menu\n\rs change SF\n\rc change CR\n\rC change CRC\n\rp change Power\n\rb change Bandwidth\n\rf change Freq\n\rt change delay between ABP beacons\n\rn change nr of PKTs to send\n\ri change delay between PKTs\n\rr change role\n\r"));
}

void SerialcurrentSettings(){
  Serial.println(F("\n\r= Current settings: ="));

  Serial.print(F("SF: "));
  Serial.println(SF);
  
  Serial.print(F("CR: 4/"));
  Serial.println(CR);

  Serial.print(F("CRC: "));
  Serial.println(CRC);
  
  Serial.print(F("Power: "));
  Serial.println(PWR);

  Serial.print(F("BW [kHz]: "));
  Serial.println(BW);

  Serial.print(F("Freq [kHz]: "));
  Serial.println(FREQ);

if (ROLE != 2) {
    Serial.print(F("delay between ABP beacons: "));
    Serial.println(BCN_DELAY);
    
    Serial.print(F("PKTs to send: : "));
    Serial.println(NPKT);
  
    Serial.print(F("delay between PKTs: "));
    Serial.println(PKT_DELAY);
  }
  
  Serial.print(F("Role: "));
  Serial.println(ROLEv[ROLE-1]);
  
//      sprintf(sentence, "SF=%d CR=%d CRC=%d Power=%d Bandwidth=%u Freq=%u delay between ABP beacons = %u nr of PKTs to send = %u delay between PKTs = %u role = %s", SF,CR,CRC,PWR,BW,FREQ,BCN_DELAY,NPKT,PKT_DELAY,ROLEv[ROLE-1]);
//      Serial.println(sentence);

}

void SerialUIedit_settings() {

  while (Serial.available()) {
      Serial.readBytesUntil('\n', aChar, 3);
      switch (aChar[0]) {
        case 's': //increment SF value
        Iv[0]++;
        if (Iv[0] > DIMv[0]) Iv[0] = 0;
        SF = SFv[Iv[0]];
        Serial.print(F("SF: ")); Serial.println(SF);
        break;
        
        case 'c': //increment CR value
        Iv[1]++;
        if (Iv[1] > DIMv[1]) Iv[1] = 0;
        CR = CRv[Iv[1]];
        Serial.print(F("CR: 4/")); Serial.println(CR);       
        break;
        
        case 'C': //enalbe or disable CRC 
        Iv[2]++;
        if (Iv[2] > DIMv[2]) Iv[2] = 0;
        CRC = CRCv[Iv[2]];
        Serial.print(F("CRC: ")); Serial.println(CRC);        
        break;
        
        case 'p': //increment Power
        Iv[3]++;
        if (Iv[3] > DIMv[3]) Iv[3] = 0;
        PWR = PWRv[Iv[3]];   
        Serial.print(F("Power: ")); Serial.println(PWR);     
        break;        
        
        case 'b': //increment Bandwidth value
        Iv[4]++;
        if (Iv[4] > DIMv[4]) Iv[4] = 0;
        BW = BWv[Iv[4]];
        Serial.print(F("Bandwidth: ")); Serial.println(BW);       
        break;

        case 'f': //increment Frequence value
        Iv[5]++;
        if (Iv[5] > DIMv[5]) Iv[5] = 0;
        FREQ = FREQv[Iv[5]];
        Serial.print(F("Freq: ")); Serial.println(FREQ);        
        break;

        case 't': //increment interval between beacons in ABP mode
        Iv[7]++;
        if (Iv[7] > 10) Iv[7] = 1;
        BCN_DELAY=BCN_DELAYv[(Iv[7]-1)];
        Serial.print(F("BCN interval: [seconds]")); Serial.println(BCN_DELAY);       
        break;

        case 'n': //increment number of packets sent in PKT generator mode
        Iv[8]++;
        if (Iv[8] > 8) Iv[8] = 1;
        NPKT=(PKT_DELAYv[(Iv[8]-1)]/10); //reusing same vector of aviable choice
        Serial.print(F("PKTs to send: ")); Serial.println(NPKT);
        break;

        case 'i': //increment interval between packets in PKT generator mode
        Iv[9]++;
        if (Iv[9] > 8) Iv[9] = 1;
        PKT_DELAY = PKT_DELAYv[(Iv[9]-1)];   
        Serial.print(F("PKT interval [ms]: ")); Serial.println(PKT_DELAY);     
        break;
        
        case 'r': //change role
        ROLE++;
        if (ROLE > 3) ROLE = 1;
        Serial.print(F("Role: ")); Serial.println(ROLEv[ROLE-1]);
        Serial.print(F("To effectively change role remember to power cycle the board after saving"));       
        break;

        case 'S': //save settings to eeprom
        save_settings();
        SerialcurrentSettings();
        if(RadioInit()) Serial.println(F("Settings saved and LoRa module OK!\n")); 
        RadioSet();
        sconf=0; 
        break;

        case 'h': //print help menu with aviable commands
        aviableCommands();
        break;

        case 'H': //print current settings
        SerialcurrentSettings();
        Serial.println(F("after changes REMEMBER to send 'S' to init radio module with new parameters and save them to eeprom\n"));
        break;

        default:
        aviableCommands();
        break;

      }
     }
  }      

  SerialUI() {
    Serial.println(F("========================="));
    aviableCommands();
    while(sconf){
      SerialUIedit_settings();
    }
  }
