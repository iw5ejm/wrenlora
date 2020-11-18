
//vectors to store the aviable choices for each parameter
const uint8_t SFv[] = {7, 8, 9, 10, 11, 12}; //aviable choices for SF
const uint8_t CRv[] = {5, 6, 7, 8}; //aviable choices for CR..and so on
const uint8_t CRCv[] = {0, 1};
const uint8_t PWRv[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
const long    BWv[] = {125000L, 250000L, 500000L};
const long    FREQv[] = {433175000L, 433375000L, 433575000L, 868100000L, 868300000L, 868500000L}; //mandatory first three channel for LoRaWAN EU433 and EU868 
const uint8_t DIMv[] = {5, 3, 1, 20, 2, 5}; //lengths-1 of each upper vector, used to manage the scroll
const char    *ROLEv[] = {"PKT GENERATOR", "RECEIVER", "ABP BEACON"};
const uint16_t BCN_DELAYv[] = {1, 3, 5, 10, 20, 60, 300, 600, 900, 1800}; //aviable choice for delay [in seconds] between packets in ABP beacon mode
const uint16_t PKT_DELAYv[] = {100, 300, 500, 1000, 1500, 2000, 2500, 5000};  //aviable choice for time delay between the transmission of packets

int8_t    Iv[] = {0, 0, 0, 13, 0, 0, 0, 0, 2, 2}; //index to scroll the aviable choice for each setting

//-----------------------------------------------------------------------------------------------
