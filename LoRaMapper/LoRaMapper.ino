
#include <EEPROM.h>
#include <iarduino_GPS_NMEA.h>                     //  Подключаем библиотеку для расшифровки строк протокола NMEA получаемых по UART.
#include <iarduino_GPS_ATGM336.h>                  //  Подключаем библиотеку для настройки параметров работы GPS модуля ATGM336.
#include "uCRC16Lib.h"
#include <SPI.h>
#include "TFT.h"
#include <avr/wdt.h>

#define BR_OFF -1
#define TO_PC 0
#define BR_GPS 3
#define BR_EBYT 2
#define BR_RAK 1

#define batVoltMax 379
#define batVoltMin 310
#define countBytesPcmax 240  // максимальное кол-во принимаемых байт
#define VER_X_YY d1
#define VER_Y_XY d3
#define VER_Y_YX d8

#define cs 53  //nano - 10 // пин для подключения CS
#define dc 48  //nano - 9 // пин для подключения A0
#define rst 46 //nano - 8 // пин для подключения reset
#define timeoutPlusLCD 15


TFT screen = TFT(cs, dc, rst);
iarduino_GPS_NMEA    gps;                          //  Объявляем объект gps         для работы с функциями и методами библиотеки iarduino_GPS_NMEA.
iarduino_GPS_ATGM336 SettingsGPS;                  //  Объявляем объект SettingsGPS для работы с функциями и методами библиотеки iarduino_GPS_ATGM336.

HardwareSerial* serialPorts[] = { &Serial, &Serial1, &Serial2, &Serial3 };


  // Массив для хранения сообщения
  char message[40];
  const uint16_t CRC16_TABLE[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

byte receivedData[countBytesPcmax];   // массив полученных
byte receivedCount = 0;               // кол-во полученных байт

byte toSendData[countBytesPcmax];    // массив подготовленных к отправке данных
byte toSendCount = 0;                // кол-во подготовленных к отправке байт

  float longitude;
  float latitude;
  byte accuracyX;
  byte accuracyZ;
  byte accuracyY;
  byte activeSatellites;
  byte observedSatellites;
  byte currentMessageNumber = 20;
  byte totalMessages = 20;
  int batteryLevel = 354;
  unsigned int crcSum;

int sensState = BR_OFF;    // Состояние bridge
bool ebytePullFlag = true;
bool gpsIstalled = true;
short dataDirection = 0;      // Направление отправки подготовленных данных
short currenModule = BR_EBYT;
int deviceNumber = 0;
unsigned long humanClock = 0; // Счетчик секунд (общий для программы)
int subClockSend = 0;
int subClockLCD = 0;
bool foundPartMsg = false; //Если в предыдущем сообщении часть посылки
bool devState [3]; // для отображения на экране статуса 
int devTimer [3]; // для отображения на экране счетчиков
int lastSendTimer[5];
int lastReceiveTimer[5];
int workTimer = 0;
float voltage = 0.0;
int voltageCharge = 0;
int voltageChargeEnd = 0;
int chargePercent = 0;
int rssi = 0;
bool haveConnect = false;
bool haveGPS = false;
//uint8_t sec = 0;
//DEV_TIME sDev_time;

byte errorRakCount = 0; // Счетчик ошибок от модуля лоры
bool foundModule = false;
bool foundExtra = false; //command
/*************************************ASCII символы**********************/
byte ttab = 9;       // tabulation
byte LF = 10;        // перевод каретки
byte CR = 13;        // перевод каретки
byte space = 32;     // пробел
byte Exp = 33;       // знак восклицательный !
byte tab = 34;       // пробел
byte hash = 35;      // #
byte percent = 37;   // знак процент %
byte star = 42;      // знак звездочка *
byte plus = 43;      // знак плюс +
byte dash = 45;      // знак тире -
byte dot = 46;       // знак точка .
byte slash = 47;     // знак /


byte d0 = 48;  // число 0
byte d1 = 49;  // число 1
byte d2 = 50;  // число 2
byte d3 = 51;  // число 3
byte d4 = 52;  // число 4
byte d5 = 53;  // число 5
byte d6 = 54;  // число 6
byte d7 = 55;  // число 7
byte d8 = 56;  // число 8
byte d9 = 57;  // число 9

byte colon = 58;     // знак двоеточие :
byte menshe = 60;    // знак меньше <
byte equal = 61;     // знак равно =
byte bolshe = 62;    // знак больше >
byte question = 63;  // знак вопрос ?

byte A = 65;
byte B = 66;
byte C = 67;
byte D = 68;
byte E = 69;
byte F = 70;
byte G = 71;
byte H = 72;
byte I = 73;
byte K = 75;
byte L = 76;
byte M = 77;
byte N = 78;
byte O = 79;
byte P = 80;
byte Q = 81;
byte R = 82;
byte S = 83;
byte T = 84;
byte U = 85;
byte V = 86;
byte W = 87;
byte X = 88;
byte Y = 89;

byte buttomLine = 95;     // знак _ (нижнее подчеркивание)

byte a = 97;
byte b = 98;
byte c = 99;
byte d = 100;
byte e = 101;
byte f = 102;
byte g = 103;
byte r = 114;
byte i = 105;
byte bukvaISmall = 105;
byte t = 116;
byte m = 109;
byte s = 115;
byte h = 104;
byte k = 107;
byte u = 117;
byte o = 111;
byte v = 118;
byte l = 108;
byte w = 119;
byte p = 112;
byte n = 110;
byte x = 120;


/*************************************Шаблоны для сообщений модуля лоры**********************/
byte msgPrefix[] = { A, T, plus };

byte setP2PMode[] = { N, W, M, equal, d0};
byte setFreq[] = { P,F,R,E,Q,equal,d8,d6,d8,d1,d0,d0,d0,d0,d0};
byte setPSF[] = { P,B,W, equal, d1, d2, d5};

byte setP2pParameters [] = {P, d2, P ,equal, d8,d6,d8, d1, d0, d0, d0, d0, d0, colon, d7, colon, d1, d2, d5, colon, d0, colon, d8, colon, d2, d2};
byte setReciveMode[] = { P, R, E, C, V, equal, d6, d5, d5, d3, d5 };
byte setTransmiteMode[] = {P, R, E, C, V, equal, d0};
byte sendP2PWord [] = {P, S, E, N, D, equal};
byte sendMsg[] = {P, S, E, N, D, equal, d0, d0, C, d8, d1, d9, d0, d0, d0, d2, d0, d0, d1, d0, d0, d1, d0, d0, d0, d0, d0, d0, d0, d0, d0, d0, d0, deviceNumber+48};
//byte sendMsg[] = {P, S, E, N, D, equal, d0, d0, C, d8};
byte ebytePrefix [] = {E, B, Y, T, E, colon};
byte rakPrefix [] = {R, A, K, colon};
byte msgPostfix[] = { 13, 10 };


/*************************************Шаблоны для сообщений ответов**********************/
//at+set_config=lorap2p:868100000:11:0:1:8:10 [43 symb]
//byte msgRakConfig [] = {a,t,plus,s,e,t,buttomLine,c,o,n,f,i,g,equal,l,o,r,a,p,d2,p,colon,d8,d6,d8,d1,d0,d0,d0,d0,d0,colon,d1,d1,colon,d0,colon,d1,colon,d8,colon,d1,d0,CR };
//byte msgRakConfig [] = {a,t,plus,s,e,t,buttomLine,c,o,n,f,i,g,equal,l,o,r,a,p,d2,p,colon,d8,d6,d8,d1,d0,d0,d0,d0,d0,colon,d1,d1,colon,d0,colon,d1,colon,d8,colon,d1,d0,'\r', '\n' };
//byte msgRakConfig [] = {97, 116, 43, 115, 101, 116, 95, 99, 111, 110, 102, 105, 103, 61, 108, 111, 114, 97, 112, 50, 112, 58, 56, 54, 56, 49, 48, 48, 48, 48, 48, 58, 49, 49, 58, 48, 58, 49, 58, 56, 58, 49, 48, 13, 10};
  byte msgRakVersion [] = {a,t,plus,v,e,r,s,i,o,n,CR };
//byte msgRakConfig [] = "at+set_config=lorap2p:868100000:11:0:1:8:10\r\n";
byte msgRakRceivePackage[] = { a, t, plus, r, e, c, v, equal};
bool firstSend = true;
byte msgRakTimeoutRecieMode[] = { plus, E, V, T, colon, P, d2, P, space};
byte msgRakOkVersion[] = {O,K, space, V, d3};
byte rcwWord [] = {R, C, V, colon};

byte msgRakRceivePackagePart[] = { plus, E, V, T, colon, R, X};
byte msgRakAtError[] = { A, T, buttomLine, E, R, R, O};
byte msgRakAtErrorB[] = {CR, LF, A, T, buttomLine, B};
byte msgRakOk[] = {CR, LF, O, K};
byte rsssiWord[] = {R,S,S,I};
byte commaSpase[] = {',', space};
byte spacers[] = {space, space, space};
//AT_BUSY_ERROR


byte VersionSW[] = { S, W, colon, VER_X_YY, dot, VER_Y_XY, VER_Y_YX};
byte ok[]   = {O,k,CR};          // Ok
byte error[] = {e,r,r,CR};       // Error 

byte gpson [] = {G, P, S, O, N, CR};    //Bridge URT3
byte gpsoff [] = {G, P, S, O, F, F, CR};   
byte ebyton [] = {E, B, Y, T, E, O, N, CR};    //Bridge URT2
byte ebytoff [] = {E, B, Y, T, E, O, F, F, CR};   
byte ebytePull [] = {E, B, Y, T, E, P, U, L, L, CR};   
byte rakon [] = {R, A, K, O, N, CR};    //Bridge URT1
byte rakoff [] = {R, A, K, O, F, F, CR}; 
byte gpsDisable [] = {G, P, S, D, I, S, A, B, L, E, CR}; 
byte gpsEnable [] = {G, P, S, E, N, A, B, L, E, CR}; 
byte forceSend [] = {F, O, R, C, E, S, E, N, D, CR};


byte srev[] = { S, R, E, V, question, CR };    //SREV?..........//like IGM 13
byte reboot[] = { R, E, B, O, O, T, CR };
uint8_t lostArr[] = {L, O, S, T};
uint8_t okArr[] = {O, K};
/*************************************Для дисплея**********************/
uint8_t Str_ArrayMyZero[17] = {v, VER_X_YY, dot, VER_Y_XY, VER_Y_YX, L, o, R, a, space, M, a, p, p, e, r};
uint8_t *pStrMyZero = Str_ArrayMyZero;

uint8_t Str_ArrayMyOne[16] = {G, P, S, colon, question, question,  question, question, space, space, space, space};
uint8_t *pStrMyOne = Str_ArrayMyOne;

uint8_t Str_ArrayMyTwo[16] = {space, space, space, space, space, space,  space, space, space, space, space, space};
uint8_t *pStrMyTwo = Str_ArrayMyTwo;

uint8_t Str_ArrayMyThree[16] = {space, space, space, space, space, space,  space, space, space, space, space, space};
uint8_t *pStrMyThree = Str_ArrayMyThree;

uint8_t Str_ArrayMyFour[16] = {space, space, space, space, space, space,  space, space, space, space, space, space};
uint8_t *pStrMyFour = Str_ArrayMyFour;

uint8_t Str_ArrayMyFive[16] = {space, space, space, space, space, space,  space, space, space, space, space, space};
uint8_t *pStrMyFive = Str_ArrayMyFive;

uint8_t Str_ArrayMyBufferLCD[16] = {space, space, space, space, space, space,  space, space, space, space, space, space};
uint8_t *pStrMyFourBufferLCD = Str_ArrayMyBufferLCD;