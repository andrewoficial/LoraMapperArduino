void lcdShow() {

  lcdLineZero();

  lcdLineOne();

  lcdLineTwo();

  lcdLineThree();

  lcdLineFour();

  lcdLineFive();

}



void lcdLineZero(){
    int tmp = 0;

  //chargePercent = map(voltage, 2.9, 4.2, 0, 100);
  chargePercent = voltage * 100;



  if(chargePercent < batVoltMin){
    chargePercent = batVoltMin;
  }

  if(chargePercent > batVoltMax){
    chargePercent = batVoltMax;
  }


    
  chargePercent = map(chargePercent, batVoltMin, batVoltMax, 0, 100);
  Str_ArrayMyZero[4] = space;
  /*
  Dev1 - 301 - 372
  Dev2 - 305 - 410
  */
  if (voltageCharge < 100) {  //Идет зарядка
    
    NumericToByteArr(Str_ArrayMyBufferLCD, chargePercent);
    Str_ArrayMyZero[0] = Str_ArrayMyBufferLCD[2];
    Str_ArrayMyZero[1] = Str_ArrayMyBufferLCD[3];
    Str_ArrayMyZero[2] = Str_ArrayMyBufferLCD[4];
    Str_ArrayMyZero[3] = percent;    

    Str_ArrayMyZero[5] = C;
    Str_ArrayMyZero[6] = h;
    Str_ArrayMyZero[7] = a;
    Str_ArrayMyZero[8] = r;
    Str_ArrayMyZero[9] = g;
    Str_ArrayMyZero[10] = bukvaISmall;
    Str_ArrayMyZero[11] = n;
    Str_ArrayMyZero[12] = g;
    Str_ArrayMyZero[13] = space;
    Str_ArrayMyZero[14] = space;
    Str_ArrayMyZero[15] = space;
    //Str_ArrayMyZero[16] = space;
    //Str_ArrayMyZero[17] = space;  

  }

  if (voltageChargeEnd < 100) {  //Зарядка завершена
    NumericToByteArr(Str_ArrayMyBufferLCD, chargePercent);
    Str_ArrayMyZero[0] = Str_ArrayMyBufferLCD[2];
 
    Str_ArrayMyZero[1] = Str_ArrayMyBufferLCD[3];
    Str_ArrayMyZero[2] = Str_ArrayMyBufferLCD[4];

    Str_ArrayMyZero[3] = percent;  

    Str_ArrayMyZero[5] = C;
    Str_ArrayMyZero[6] = h;
    Str_ArrayMyZero[7] = a;
    Str_ArrayMyZero[8] = r;
    Str_ArrayMyZero[9] = g;
    Str_ArrayMyZero[10] = e;
    Str_ArrayMyZero[11] = space;
    Str_ArrayMyZero[12] = d;
    Str_ArrayMyZero[13] = o;
    Str_ArrayMyZero[14] = n;
    Str_ArrayMyZero[15] = e;
    //Str_ArrayMyZero[16] = space;
    //Str_ArrayMyZero[17] = space;  
  }

  if (voltageChargeEnd > 200 && voltageCharge > 200) {  //Рабочий режим, показывать зарядку в процентах
    NumericToByteArr(Str_ArrayMyBufferLCD, chargePercent);
    Str_ArrayMyZero[0] = Str_ArrayMyBufferLCD[2];
 
    Str_ArrayMyZero[1] = Str_ArrayMyBufferLCD[3];
    Str_ArrayMyZero[2] = Str_ArrayMyBufferLCD[4];
    Str_ArrayMyZero[3] = percent;  
    Str_ArrayMyZero[5] = L;
    Str_ArrayMyZero[6] = o;
    Str_ArrayMyZero[7] = R;
    Str_ArrayMyZero[8] = a;
    Str_ArrayMyZero[9] = space;
    Str_ArrayMyZero[10] = F;
    Str_ArrayMyZero[11] = bukvaISmall;
    Str_ArrayMyZero[12] = n;
    Str_ArrayMyZero[13] = d;
    Str_ArrayMyZero[14] = e;
    Str_ArrayMyZero[15] = r;
  }
  screen.setTextSize(1);
  screen.fillRect(0, 0, 250, 10, ST7735_WHITE); // стираем старое    x y width hight //(355, 117, 90, 23, TFT_BLACK)
  screen.textWrap(pStrMyZero, 0, 0);
  screen.setTextSize(2);
}

void lcdLineOne(){
  Str_ArrayMyBufferLCD[0] = G;
  Str_ArrayMyBufferLCD[1] = P;
  Str_ArrayMyBufferLCD[2] = S;
  Str_ArrayMyBufferLCD[3] = colon;
    byte buferArray[] = { d0, d0, d0, d0, d0 };
    int gpsPercent = map(activeSatellites, 0, 21, 0, 100);
    NumericToByteArr(buferArray, gpsPercent);
    //Serial.print("activeSatellites: ");
    //Serial.println(activeSatellites);
  if(! gpsIstalled){
    Str_ArrayMyBufferLCD[4] = O;
    Str_ArrayMyBufferLCD[5] = F;
    Str_ArrayMyBufferLCD[6] = F;
    Str_ArrayMyBufferLCD[7] = space;
    Str_ArrayMyBufferLCD[8] = space;  
  }else if(haveGPS){
    Str_ArrayMyBufferLCD[4] = space;
    Str_ArrayMyBufferLCD[5] = buferArray[2];
    Str_ArrayMyBufferLCD[6] = buferArray[3];
    Str_ArrayMyBufferLCD[7] = buferArray[4];
    Str_ArrayMyBufferLCD[8] = percent;
  }else{
    Str_ArrayMyBufferLCD[4] = L;
    Str_ArrayMyBufferLCD[5] = O;
    Str_ArrayMyBufferLCD[6] = S;
    Str_ArrayMyBufferLCD[7] = T;
    Str_ArrayMyBufferLCD[8] = space;
  }
  
  for (i = 1; i < 9; i++) {
    Str_ArrayMyOne[i] = compareAndUpdateSymbol(Str_ArrayMyOne[i],Str_ArrayMyBufferLCD[i], i ,13);
  }

}

void lcdLineTwo(){
  byte buferArray[] = { d0, d0, d0, d0, d0 };
  NumericToByteArr(buferArray, lastSendTimer[BR_EBYT]);

  Str_ArrayMyBufferLCD[0] = d8;
  Str_ArrayMyBufferLCD[1] = X;
  Str_ArrayMyBufferLCD[2] = X;
  Str_ArrayMyBufferLCD[3] = colon;
  Str_ArrayMyBufferLCD[4] = buferArray[0];
  Str_ArrayMyBufferLCD[5] = buferArray[1];
  Str_ArrayMyBufferLCD[6] = buferArray[2];
  Str_ArrayMyBufferLCD[7] = buferArray[3];
  Str_ArrayMyBufferLCD[8] = buferArray[4];
  Str_ArrayMyBufferLCD[9] = bolshe;
  for (i = 0; i < 10; i++) {
    Str_ArrayMyTwo[i] = compareAndUpdateSymbol(Str_ArrayMyTwo[i],Str_ArrayMyBufferLCD[i], i ,36);
  }
}

void lcdLineThree(){
  byte buferArray[] = { d0, d0, d0, d0, d0 };
  NumericToByteArr(buferArray, lastSendTimer[BR_EBYT]);
  Str_ArrayMyBufferLCD[0] = d4;
  Str_ArrayMyBufferLCD[1] = X;
  Str_ArrayMyBufferLCD[2] = X;
  Str_ArrayMyBufferLCD[3] = colon;
  Str_ArrayMyBufferLCD[4] = buferArray[0];
  Str_ArrayMyBufferLCD[5] = buferArray[1];
  Str_ArrayMyBufferLCD[6] = buferArray[2];
  Str_ArrayMyBufferLCD[7] = buferArray[3];
  Str_ArrayMyBufferLCD[8] = buferArray[4];
  Str_ArrayMyBufferLCD[9] = bolshe;
  for (i = 0; i < 10; i++) {
    Str_ArrayMyThree[i] = compareAndUpdateSymbol(Str_ArrayMyThree[i],Str_ArrayMyBufferLCD[i], i ,59);
  }
}

void lcdLineFour(){
  byte buferArray[] = { d0, d0, d0, d0, d0 };
  NumericToByteArr(buferArray, lastReceiveTimer[BR_RAK]);
  Str_ArrayMyBufferLCD[0] = d8;
  Str_ArrayMyBufferLCD[1] = X;
  Str_ArrayMyBufferLCD[2] = X;
  Str_ArrayMyBufferLCD[3] = colon;
  Str_ArrayMyBufferLCD[4] = buferArray[0];
  Str_ArrayMyBufferLCD[5] = buferArray[1];
  Str_ArrayMyBufferLCD[6] = buferArray[2];
  Str_ArrayMyBufferLCD[7] = buferArray[3];
  Str_ArrayMyBufferLCD[8] = buferArray[4];
  Str_ArrayMyBufferLCD[9] = menshe;
  for (i = 0; i < 10; i++) {
    Str_ArrayMyFour[i] = compareAndUpdateSymbol(Str_ArrayMyFour[i],Str_ArrayMyBufferLCD[i], i ,82);
  }
}

void lcdLineFive(){
  byte buferArray[] = { d0, d0, d0, d0, d0 };
  NumericToByteArr(buferArray, lastReceiveTimer[BR_EBYT]);
  Str_ArrayMyBufferLCD[0] = d4;
  Str_ArrayMyBufferLCD[1] = X;
  Str_ArrayMyBufferLCD[2] = X;
  Str_ArrayMyBufferLCD[3] = colon;
  Str_ArrayMyBufferLCD[4] = buferArray[0];
  Str_ArrayMyBufferLCD[5] = buferArray[1];
  Str_ArrayMyBufferLCD[6] = buferArray[2];
  Str_ArrayMyBufferLCD[7] = buferArray[3];
  Str_ArrayMyBufferLCD[8] = buferArray[4];
  Str_ArrayMyBufferLCD[9] = menshe;
  for (i = 0; i < 10; i++) {
    Str_ArrayMyFive[i] = compareAndUpdateSymbol(Str_ArrayMyFive[i],Str_ArrayMyBufferLCD[i], i ,105);
  }
}
void lcdUpdateDevStatus(short number) {
  Str_ArrayMyThree[3] = space;
  Str_ArrayMyThree[4] = space;
  Serial.print("LCD updt:"); Serial.println(number);
  switch (number) {
    case 01:
      devTimer[0] = 0;
      devState[0] = true;
      break;
    case 02:
      devTimer[0] = 0;
      devState[0] = true;
      break;

    default:
      Str_ArrayMyThree[3] = number / 10 + 48;
      Str_ArrayMyThree[4] = number % 10 + 48;
      devTimer[2] = number;
      // if nothing else matches, do the default
      break;
  }
}
uint8_t compareAndUpdateSymbol(uint8_t oldSymb, uint8_t newSymb, short positionX, short positionY){
  if(oldSymb != newSymb){
    clearCurrentSymbol(positionX, positionY);
    screen.setCursor(positionX*12,positionY);
    screen.write(newSymb);
    //screen.write(oldSymb);
    //screen.text(newSymb, positionX, positionY);
  }  
  return newSymb;
}

void clearCurrentSymbol(short positionX, short positionY){
  screen.stroke(255, 255, 255);
  screen.fillRect(positionX * 12, positionY, 12, 14, ST7735_WHITE); // стираем старое    x y width hight //(355, 117, 90, 23, TFT_BLACK)
  screen.stroke(0, 0, 0);
}