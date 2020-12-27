//here the ghaphical user interface is managed

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define UP_btn A1
#define DWN_btn A2
#define SW_btn A0
#define BSTATE LOW //put LOW here to use Normally Closed to ground buttons, HIGH for Normally Open buttons
#define SDLY 350 //short delay to be used for debounce 
#define LDLY 450 //long delay to be used for debounce 

#define I2C_ADDRESS 0x3C
SSD1306AsciiWire oled; //declare the OLED object

bool    gconf = 1; //check if we are in configuration mode to edit the parameters
bool    pag = 0;   //index for the page

//vectors to store the aviable choices for each parameter
const uint8_t SFv[] = {7, 8, 9, 10, 11, 12}; //aviable choices for SF
const uint8_t CRv[] = {5, 6, 7, 8}; //aviable choices for CR..and so on
const uint8_t CRCv[] = {0, 1};
const uint8_t PWRv[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
const uint16_t    BWv[] = {125, 250, 500};
const long    FREQv[] = {433175000L, 433375000L, 433575000L, 868100000L, 868300000L, 868500000L}; //mandatory first three channel for LoRaWAN EU433 and EU868 
const uint8_t DIMv[] = {5, 3, 1, 20, 2, 5}; //lengths-1 of each upper vector, used to manage the scroll
const char    *ROLEv[] = {"PKT GENERATOR", "RECEIVER", "ABP BEACON"};
const uint16_t BCN_DELAYv[] = {1, 3, 5, 10, 20, 60, 300, 600, 900, 1800}; //aviable choice for delay [in seconds] between packets in ABP beacon mode
const uint16_t PKT_DELAYv[] = {100, 300, 500, 1000, 1500, 2000, 2500, 5000};  //aviable choice for time delay between the transmission of packets

const void *pippo[] = {&SF, &CR, &CRC, &PWR, &BW, &FREQ, &pag}; //an array of pointers to a mixed type of elements storing the global LoRa parameters
const void *pluto[] = {SFv, CRv, CRCv, PWRv, BWv, FREQv}; //an array of pointers to the lists of aviable choice for each LoRa parameter

uint8_t   *b; //support array used in parameters selection
long      *B; //very long support array
uint8_t   menuCount = 6; //index for the selected menù
int8_t    Iv[] = {0, 0, 0, 13, 0, 0, 0, 0, 2, 2}; //index to scroll the aviable choice for each setting

//-----------------------------------------------------------------------------------------------



//print the static part of the config menù page 1
void staticMenu1() {
  oled.clear(); 
  oled.println(F(" Enter LoRa settings"));

  oled.setCursor(10,1); //prepare to print the text 10 pixel from the left margin of the display, on the first line
  oled.print(F("SF:"));
  oled.setCol(80); //prepare to print the text 80 pixel from the left margin of the display
  oled.println(SF);
//oled.println( *((uint8_t *)pippo[0]) ); //same thing of the upper line but with the correct dereferencing syntax: in pippo[0] there is a pointer to SF
  
  oled.setCursor(10, 2); //prepare to print the text 10 pixel from the left margin of the display, on the second line
  oled.print(F("CR:"));
  oled.setCol(68);
  oled.print(F("4/"));
  oled.setCol(80);
  oled.println(CR);

  oled.setCursor(10, 3);
  oled.print(F("CRC:"));
  oled.setCol(80);
  oled.println(CRC);
  
  oled.setCursor(10, 4);
  oled.print(F("Power:"));
  oled.setCol(80);
  oled.println(PWR);

  oled.setCursor(10, 5);
  oled.print(F("BW"));
  oled.setCol(80);
  oled.println(BW);

  oled.setCursor(10, 6);
  oled.print(F("Freq:"));
  oled.setCol(80);
  oled.println(FREQ);

  oled.setCursor(0, 7);
  oled.print(F("> menu page"));
  oled.setCol(120);
  oled.println("1");
  
}

//scroll the menù and edit settings value
void menuCheck1() {
  //when you press the SW button the cursor scrolls each line of the menu  
  if (digitalRead(SW_btn) == BSTATE) {
    oled.clearField(0,menuCount+1,1);
    menuCount++; //update the index of the current active line of the menu
    if (menuCount > 6) menuCount = 0; //we go back on top
    oled.setRow(menuCount+1);
    oled.write(">");
  }

  //When you press the UP button the parameter of the current line is incremented or the selected command executed (ex. SAVE AND EXIT)
  //The parameter to update is identified by the pippo pointer.
  //The value to assign to the parameter is choosen from the vector of the aviable choice, selected by the pluto pointer.
  //Iv vector keep track of the current selected value for each parameter
  if (digitalRead(UP_btn) == BSTATE) {
    oled.clearField(80,menuCount+1,3);
    oled.setCursor(80,menuCount+1); 
    Iv[menuCount]++;
    if (Iv[menuCount] > DIMv[menuCount]) Iv[menuCount] = 0; 
     
    if (menuCount == 4 || menuCount == 5 ) {//check if we are editing a long or a byte parameter
      B=pluto[menuCount];
      *((long*)pippo[menuCount]) = B[Iv[menuCount]];
      oled.println(*((long*)pippo[menuCount]));
      }
    else if (menuCount == 6) {//change page
      delay(100);
      pag=1;
      }
      
    else {
      b=pluto[menuCount];
      *((uint8_t*)pippo[menuCount]) = b[Iv[menuCount]];
      oled.println(*((uint8_t*)pippo[menuCount]));
      }
    }

    //when you press the DOWN button the parameter of the current line is decremented or the selected command executed.
    if (digitalRead(DWN_btn) == BSTATE) {
    oled.clearField(80,menuCount+1,3);
    oled.setCursor(80,menuCount+1);
    Iv[menuCount]--;
    if (Iv[menuCount] < 0) Iv[menuCount] = DIMv[menuCount];
    
    if (menuCount == 4 || menuCount == 5 ) {//check if we are editing a long or a byte parameter
      B=pluto[menuCount];
      *((long*)pippo[menuCount]) = B[Iv[menuCount]];
      oled.println(*((long*)pippo[menuCount]));
      }
    else if (menuCount == 6) {//change page
      delay(100); //add a little time for more debounce
      pag=1;
      }
      
    else {
      b=pluto[menuCount];
      *((uint8_t*)pippo[menuCount]) = b[Iv[menuCount]];
      oled.println(*((uint8_t*)pippo[menuCount]));
      }
  }

}

//print the static part of the config menù page 2
void staticMenu2() {
  oled.clear(); 
  oled.println(F(" Enter Wren settings"));

  oled.setCursor(10,2);
  oled.print(F("Role:"));
  
  oled.setCursor(10, 3);
  oled.println( ROLEv[ROLE-1] ); //same thing of the upper line but with the correct dereferencing syntax: in pippo[0] there is a pointer to SF
  if (ROLE == 3) {oled.setCursor(10, 4); oled.print(F("PKT interval: ")); oled.print(BCN_DELAY); oled.println("\"");} //if in beacon mode print the delay between transmissions
  
  oled.setCursor(10, 5);
  oled.print(F("SAVE & EXIT MENU"));

  oled.setCursor(0, 7);
  oled.print(F("> menu page"));
  oled.setCol(120);
  oled.println("2");

}



void menuCheck2() {
  //when you press the SW button the cursor scroll each line of the menu  
  if (digitalRead(SW_btn) == BSTATE) {
    oled.clearField(0,menuCount+1,1); //delete the cursor to move on the next lint

    if (ROLE == 3 && menuCount == 2) menuCount=3;
    else if (menuCount == 3) menuCount=4;
    else menuCount=menuCount+2; //update the index of the current active line of the menu
    if (menuCount > 6) menuCount = 2; //we go back on top
    oled.setRow(menuCount+1);
    oled.write(">");
  }

   //when you press the UP button the parameter of the current line is incremented or the current function selected
  if (digitalRead(UP_btn) == BSTATE) {     
    if (menuCount == 2) {
      oled.setCursor(10, 3);
      oled.clearToEOL();
      ROLE++;
      if (ROLE > 3) ROLE = 1;
      oled.println( ROLEv[ROLE-1] );
      oled.setCursor(10, 4);
      if (ROLE == 3) {oled.print(F("PKT interval: ")); oled.println(BCN_DELAY);}
      else oled.clearToEOL();
      
      }

    else if (menuCount == 3) {//change delay between packets in beacon mode
      oled.setCursor(94, 4);
      oled.clearToEOL();
      Iv[7]++;
      if (Iv[7] > 10) Iv[7] = 1;
      BCN_DELAY=BCN_DELAYv[(Iv[7]-1)];
      oled.print(BCN_DELAY);
      oled.println("\"");
      }
      
    else if (menuCount == 6) {//change page
      delay(100);
      pag=0;
      }
      
    else {  //save to flash and exit configuration menu
      save_settings();
      gconf=0;
      pag=0;
      }
    }

    //when you press the DOWN button the parameter of the current line is decremented or the current function selected
    if (digitalRead(DWN_btn) == BSTATE) {
      if (menuCount == 2) {
        oled.setCursor(10, 3);
        oled.clearToEOL();
        ROLE--;
        if (ROLE < 1) ROLE = 3;
        oled.println( ROLEv[ROLE-1] );
        oled.setCursor(10, 4);
        if (ROLE == 3) {oled.print(F("PKT interval:")); oled.println(BCN_DELAY);}
        else oled.clearToEOL();
        
        }
        
      else if (menuCount == 3) {//change delay between packets in beacon mode
        oled.setCursor(94, 4);
        oled.clearToEOL();
        Iv[7]--;
        if (Iv[7] < 1) Iv[7] = 10;
        BCN_DELAY=BCN_DELAYv[(Iv[7]-1)];
        oled.print(BCN_DELAY);
        oled.println("\"");
        }
 
      else if (menuCount == 6) {//change page
        delay(100); //add a little time for more debounce
        pag=0;
        }
        
      else {  //exit configuration menu
        save_settings();
        gconf=0;
        pag=0;
        }
      }
  }


 //print the current settings for LoRa modulation and Wren packet generator
void currentSettings() {

  staticMenu1(); //reuse page 1 of the config menu to print the current settings, saving "so much" memory :)
  oled.setCursor(0,0);
  oled.clearToEOL();
  oled.println(F("   CURRENT SETTINGS"));

  oled.clear(0,128,7,7);
  oled.setCursor(10, 7);
  oled.print(F("Role:"));
  oled.setCol(50);
  oled.println(ROLEv[ROLE-1]);
  delay(2500); //time for the humans to read the current settings
 
}


//called to enter the graphic configuration mode: cycle between pages of the menu
void GUI() {
  delay(350);
  menuCount = 6;
  while(gconf) {
    staticMenu1(); //edit LoRa modulation and frequency parameters
    while(!pag) {
      menuCheck1();
      delay(250);
      }
    staticMenu2(); //edit WrenLoRa packet generator mode and ABP parameters
     while(pag) {
      menuCheck2();
      delay(250);
      }
    }

}
