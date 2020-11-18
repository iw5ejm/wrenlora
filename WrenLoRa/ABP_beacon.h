//void ABPbeacon() {
//    oled.clear();
//    oled.println(F("ABP mode entered\nsuccessfully"));
//  delay(100);
//}

//codice per RN2483. Usando questo in caso c'è da modificare solo Radio.h e Receiver per adattarlo all'uso di questo modulo
//ci sarà da mandare direttamente comandi at per impostare manualmente l'SF BW CR. conviene utilizzare sprintf come nell'rfwx nella comunicazione con
//il modem aprs.
//Sketch uses 27902 bytes (90%) of program storage space. Maximum is 30720 bytes.
//Global variables use 1190 bytes (58%) of dynamic memory, leaving 858 bytes for local variables. Maximum is 2048 bytes.

//code taken from the example of the library https://github.com/jpmeijers/RN2483-Arduino-Library
//here some more info about practical realization: https://www.thethingsnetwork.org/forum/t/how-to-build-your-first-ttn-node-arduino-rn2483/1574

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
  //CHE NE FACCIAMO DELLA SOFTWARE SERIAL PORT SERIAL2RADIO? LA CHIUDIAMO QUANDO? nel loop? perché inoltre converrebbe sostituire il while(1) con
  //while(ROLE==ABPBEACON) e mettere la funzione che aspetta user input dalla seriale fisica alla fine del main loop
  //tipo mettere if(Serial.Aviable) sconf=1; e chiamare la funzione SerialUI();



  //check communication with radio
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
    sprintf(sentence, "Hello! ", I); //in case check if myLora.tx wants an array of char with a terminator (i.e. a String..) or not
    myLora.tx(sentence); //blocking function
    oled.clear(0,128,4,4);
    oled.println(sentence);
    I++;
    delay(BCN_DELAY);    
  }

}
