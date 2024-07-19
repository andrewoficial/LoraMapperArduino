void fReboot(){
    //Serial.println("sftwr reboot");
    //WasRequired = false;
    //asm volatile("jmp 0x00");
  wdt_disable();  
  wdt_enable(WDTO_15MS);
  while (1) {
  }
}

void setSensState(int state, bool flag){
  switch (state){
    case BR_GPS:
      if(sensState <= 0 -1 && flag){
        sensState = BR_GPS;
        Serial.println("GPS BRIDGE ON");
      }
      else if(sensState == BR_GPS && (! flag)){
        sensState = BR_OFF;
        Serial.println("GPS BRIDGE OFF");
      }
      else if(sensState == state && flag){
        Serial.println("GPS BRIDGE ALREADY ON");
      }
      else if(sensState != BR_GPS && (! flag)){
        sensState = BR_OFF;
        Serial.println("GPS BRIDGE ALREADY OFF");
      }      
      else if((sensState != -1 && sensState != 0) && sensState != BR_GPS){
        Serial.print("ERROR: can`t configure bridge: ");
        printBridgeName(state);
        Serial.print(" now active bridge: ");
        printBridgeName(sensState);
      }else{
        Serial.print("ERROR: can`t configure bridge: ");
        printBridgeName(state);
        Serial.print(" now active bridge: ");
        printBridgeName(sensState);
      }
      break;
    case BR_EBYT:
      if(sensState <= 0 && flag){
        sensState = BR_EBYT;
        Serial.println("EBYT BRIDGE ON");
      }else if(sensState == BR_EBYT && (! flag)){
        sensState = BR_OFF;
        Serial.println("EBYT BRIDGE OFF");
      }
      else if(sensState == state && flag){
        Serial.println("EBYT BRIDGE ALREADY ON");
      }
      else if(sensState != BR_EBYT && (! flag)){
        sensState = BR_OFF;
        Serial.println("EBYT BRIDGE ALREADY OFF");
      }   
      else if((sensState != -1 && sensState != 0) && sensState != BR_EBYT){
        Serial.print("ERROR: can`t configure bridge: ");
        printBridgeName(state);
        Serial.print(" now active bridge: ");
        printBridgeName(sensState);
      }
      break;    
    case BR_RAK:
      if(sensState <= 0 && flag){
        sensState = BR_RAK;
        Serial.println("RAK BRIDGE ON");
      }
      else if(sensState == BR_RAK && (! flag)){
        sensState = BR_OFF;
        Serial.println("RAK BRIDGE OFF");
      }
      else if(sensState == state && flag){
        Serial.println("RAK BRIDGE ALREADY ON");
      }
      else if(sensState != BR_RAK && (! flag)){
        sensState = BR_OFF;
        Serial.println("RAK BRIDGE ALREADY OFF");
      }        
      else if((sensState != -1 && sensState != 0) && sensState != BR_RAK){
        Serial.print("ERROR: can`t configure bridge: ");
        printBridgeName(state);
        Serial.print(" now active bridge: ");
        printBridgeName(sensState);
      }
      break;          
  }
}

void printBridgeName(int num){
   switch (num){
    case TO_PC:
        Serial.print("HOST (URT0)");
        break;    
    case BR_GPS:
        Serial.print("GPS (URT3)");
        break;
    case BR_EBYT:
        Serial.print("EBYTE (URT2)");
        break;
    case BR_RAK:
        Serial.print("RAK (URT1)");
        break;   
   }
}

void fEbytePull(bool needEcho){
  if(ebytePullFlag){
    ebytePullFlag = false;
    digitalWrite(22, HIGH);
    digitalWrite(24, HIGH);
    if(needEcho){
      Serial.println("Disabled");
    }
    
  }else{
    ebytePullFlag = true;
    digitalWrite(22, LOW);
    digitalWrite(24, LOW);
    if(needEcho){
      Serial.println("Enabled");
    }
  }
}
bool isBridget(){
  return sensState > 0;
}

void fForceSend (){
  sendMessages();
}
void fServ(){
  buildAnswer(0, sizeof(VersionSW), VersionSW, true, dataDirection);
}

void fSetReceiveMode(){
    foundPartMsg = false;
    errorRakCount = 0;
    buildAnswer(0, sizeof(msgPrefix), msgPrefix, false, BR_RAK);
    buildAnswer(0, sizeof(setReciveMode), setReciveMode, false, BR_RAK);
    buildAnswer(0, sizeof(msgPostfix), msgPostfix, true, BR_RAK);
    delay(300);
    checkSomeSerialEvent(BR_RAK);
    delay(100);
}

void fSetTransmiteMode(){
    foundPartMsg = false;
    errorRakCount = 0;
    buildAnswer(0, sizeof(msgPrefix), msgPrefix, false, BR_RAK);
    buildAnswer(0, sizeof(setTransmiteMode), setTransmiteMode, false, BR_RAK);
    buildAnswer(0, sizeof(msgPostfix), msgPostfix, true, BR_RAK);
    delay(300);
    checkSomeSerialEvent(BR_RAK);
    delay(100);
}

void fRakOkMsg(){
    foundPartMsg = false;
    errorRakCount = 0;
    Serial.println("RAK:OK♥");
}

void fEchoReceived(){
        buildAnswer(0, receivedCount, receivedData, true, dataDirection);
        clearBuffers();
}

void actionRakRceivePackage(){
    foundPartMsg = true;
    errorRakCount = 0;
    int i = 0;
    short foundNumber = 0;
    for (i; i < countBytesPcmax; i++) {
      if (receivedData[i] == colon)
        break;
    }
    if(countBytesPcmax > i + 7){//Если было принято нужное кол-во байт (а не считывание незатертого буфера)
      foundNumber = ((receivedData[i + 1] - 48) * 10) + (receivedData[i + 2] - 48);
    }else{
      foundNumber = 99;
    }

    
    if (foundNumber > 0 && foundNumber < 99) {
      lcdUpdateDevStatus(foundNumber);
      foundPartMsg = false;
      //setReciverMode();
      //Serial.print("fnd0:");
      //Serial.println(foundNumber);
    }else{
      clearBuffers();
      //setReciverMode();// Не переключать (если следующая посылка сломана то бага, нужно переключение по таймеру или если неизвестная команда)
      //Serial.print("DBG0:");
      //Serial.write(receivedData[i + 1]);
      //Serial.write(receivedData[i + 2]);
      //Serial.println("--");
    }
    
}

void fRakSetFoundFlag(){
  foundModule = true;
}

void fRakRceivePackagePart(){
    //Если посылка из двух строк по частям пришла (и предыдущая посылка была норм)
    foundPartMsg = false;
    errorRakCount = 0;
    short foundNumber = 0;

    foundNumber = ((receivedData[5] - 48) * 10) + (receivedData[6] - 48);

    if (foundNumber > 0 && foundNumber < 9) {
      lcdUpdateDevStatus(foundNumber);
      clearBuffers();
      //setReciverMode();
      //Serial.print("fnd1:");
      //Serial.println(foundNumber);
    }else{
      short foundPosition = findSubStr(msgRakRceivePackagePart, sizeof(msgRakRceivePackagePart), receivedCount, 20);
      //Serial.print("Position "); Serial.println(foundPosition);
      if(foundPosition > 0){
        //Serial.print("ASCII ");
        //Serial.write(receivedData[foundPosition + 27]);
        //Serial.write(receivedData[foundPosition + 28]);
        //Serial.println();
        foundNumber = ((receivedData[foundPosition + 27] - 48) * 10) + (receivedData[foundPosition + 28] - 48);
        //Serial.print("Short ");
        //Serial.println(foundNumber);
          short rssiPosition = findSubStr(rsssiWord, sizeof(rsssiWord), receivedCount, 8);
          short rssiEndStr = findSubStr(commaSpase, sizeof(commaSpase), receivedCount, (rssiPosition+2));
          lcdUpdateDevStatus(foundNumber);
          rssiPosition += 1;
          short rssiNumberLength = rssiEndStr -rssiPosition;

            Serial.print("dev: "); Serial.print(foundNumber); Serial.print(" vlt "); Serial.write(receivedData[foundPosition + 11]);Serial.write(receivedData[foundPosition + 12]);Serial.write(receivedData[foundPosition + 13]);
              Serial.print(" batPerc "); Serial.write(receivedData[foundPosition + 2]);Serial.write(receivedData[foundPosition + 3]);Serial.write(receivedData[foundPosition + 4]);
                Serial.print(" RSSI ");          
            for(i = 0; i < (rssiNumberLength-1); i++){
              Str_ArrayMyThree[5+i] = compareAndUpdateSymbol(Str_ArrayMyThree[5+i],receivedData[rssiPosition+i],(5+i),90);
              Serial.write(receivedData[rssiPosition+i]);
            }
            Str_ArrayMyThree[5+i] = compareAndUpdateSymbol(Str_ArrayMyThree[5+i],spacers[1],(5+i),90);
            i++;
            Str_ArrayMyThree[5+i] = compareAndUpdateSymbol(Str_ArrayMyThree[5+i],spacers[1],(5+i),90);
            
            updateLoraMsgRSSI(rssiPosition, true);
            updateLoraMsgTimer(true);
            echoReceivedMSG();
      }else{
        Serial.println();
        //setReciverMode();//Переключить даже если номер не нашелся
        Serial.print("DBG1:");
        buildAnswer(0, receivedCount, receivedData, true, 0);
        Serial.println();
        clearBuffers();
      }

    }  
}

void fRakAtError(){
    foundPartMsg = false;
    delay(3000);
}

