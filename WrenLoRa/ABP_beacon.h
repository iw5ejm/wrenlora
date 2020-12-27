/*
 * ABP: initABP(String addr, String AppSKey, String NwkSKey);
 * Paste keys from the TTN console here:
 */
const char *devAddr = "02017201";
const char *nwkSKey = "AE17E567AECC8787F749A62F5541D522";
const char *appSKey = "8D7FFEF938589D95AAD928C2E2E7E48F";

/*
 * OTAA: initOTAA(String AppEUI, String AppKey);
 * If you are using OTAA, paste the keys from the TTN console here:
 */
//const char *appEui = "70B3D57ED00001A6";
//const char *appKey = "A23C96EE13804963F8C2BD6285448198";


void ABPinit() {
  //check communication with radio module
  String hweui = myLora.hweui();
  while(hweui.length() != 16)
  {
    //Serial.println(F("Communication with RN2xx3 unsuccessful. Power cycle the board."));
   // Serial.println(hweui);
    delay(10000);
    hweui = myLora.hweui();
  }

  //print out the HWEUI so that we can register it via ttnctl
  //Serial.println(F("When using TTN, register this DevEUI: "));
  //Serial.println(myLora.hweui());
  //Serial.println(myLora.sysver()); //print RN2483 firmware version
  oled.println("LoRa module OK!");
  
  //configure your keys and join the network
  bool join_result = false;
  join_result = myLora.initABP(devAddr, appSKey, nwkSKey); //join via ABP
  //join_result = myLora.initOTAA(appEui, appKey);         //join via OTAA

  while(!join_result)
  { 
    //Serial.println(F("Unable to join TTN!"));
    oled.println(F("Unable to join TTN!"));
    delay(30000); //delay half a minute before retry
    join_result = myLora.init();
  }
  //Serial.println(F("TTN successfully joined"));
  oled.println(F("TTN joined!\n"));
}

void ABPbeacon() {
  
  char sentence[35];
  uint16_t I=1;
  
  oled.clear();
  oled.println(F("     ABP BEACON"));
  
  ABPinit();
  
  //sprintf(sentence, "   TXing a packet\n     every %d\"",BCN_DELAY);
  //Serial.println(sentence);
  oled.print("   TXing a packet\n     every "); oled.print(BCN_DELAY); oled.println("\"");
  
  while(1){
    sprintf(sentence, "Hello! ", I); 
    myLora.tx(sentence); //blocking function
    oled.clear(0,128,4,4);
    oled.println(sentence);
    I++;
    delay(BCN_DELAY);    
  }

}
