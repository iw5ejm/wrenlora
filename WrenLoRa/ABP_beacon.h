//still to be tested

bool join_result = 0;

/*
 * ABP: initABP(String addr, String AppSKey, String NwkSKey);
 * Paste keys from the TTN console here:
 */
const char *devAddr = "26013F20";
const char *nwkSKey = "24F0A1FF2BC47B814820FC28B15504C5";
const char *appSKey = "7119DEDC4AD61CE195EB4741EBB72234";

/*
 * OTAA: initOTAA(String AppEUI, String AppKey);
 * If you are using OTAA, paste the keys from the TTN console here:
 */
//const char *appEui = "***************";
//const char *appKey = "***************";


void ABPinit() {
  //check communication with radio module
  if(RadioInit()) oled.println("LoRa module OK!");
  
  //print out the HWEUI so that we can register it via ttnctl
  Serial.println(F("Use the HWeui as DevEUI in the TTN console to register the device"));
  Serial.println(myLora.sysver()); //print RN2483 firmware version
  
  //configure your keys and join the network
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
  
  uint16_t I=1;
  
  oled.clear();
  oled.println(F("     ABP BEACON"));
  
  ABPinit();
  
  //sprintf(sentence, "   TXing a packet\n     every %d\"",BCN_DELAY);
  //Serial.println(sentence);
  oled.print("   TXing a packet\n     every "); oled.print(BCN_DELAY); oled.println("\"");
  
  while(1){
    if(PL) sprintf(sentence, "%s %3d", payloadA, I); else sprintf(sentence, "%s %3d", payloadB, I); 
    myLora.tx(sentence); //blocking function
    oled.clear(15,128,7,7);
    oled.print(I); oled.println(F(" PKT TXed"));
    I++;
    delay(BCN_DELAY*1000L);    
  }

}
