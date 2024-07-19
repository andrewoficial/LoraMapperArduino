// ---------------------------------------------------- Инициализация доступных команд
void initCommand() {
  foundExtra = false;
  initExtraCommands();
  if (isBridget() || foundExtra) {
    return;
  }else{
    if (FindCmd(reboot, sizeof(reboot)) == true) {
      fReboot();
    } else if (FindCmd(srev, sizeof(srev)) == true) {
      fServ();
    } else if (FindCmd(gpsDisable, sizeof(gpsDisable)) == true) {
      fSaveGPSstate(false);
    } else if (FindCmd(gpsEnable, sizeof(gpsEnable)) == true) {
      fSaveGPSstate(true);
    } else if (FindCmd(forceSend, sizeof(forceSend)) == true) {
      fForceSend();
    } else if (FindCmd(msgRakTimeoutRecieMode, sizeof(msgRakTimeoutRecieMode)) == true) {
      Serial.println("Reset RAK receive timer");
      echoRak();
      fSetReceiveMode();

    } else if (FindCmd(msgRakRceivePackage, sizeof(msgRakRceivePackage)) == true) {
      //actionRakRceivePackage();
      echoRak();
      fSetReceiveMode();
    } else if (FindCmd(msgRakOk, sizeof(msgRakOk)) == true) {
      fRakOkMsg();
      //echoRak();
    } else if (FindCmd(msgRakRceivePackagePart, sizeof(msgRakRceivePackagePart)) == true) {
      //fRakRceivePackagePart();
      lastReceiveTimer[BR_RAK] = 0;
      echoRak();
      fSetReceiveMode();
    } else if (FindCmd(msgRakOkVersion, sizeof(msgRakOkVersion)) == true && foundPartMsg) {
      //fRakSetFoundFlag();
      
      echoRak();
    } else if (FindCmd(msgRakAtError, sizeof(msgRakAtError)) == true) {
      //fRakAtError();
      echoRak();
    } else {
      if (dataDirection == BR_EBYT) {
        lastReceiveTimer[BR_EBYT] = 0;
        buildAnswer(0, sizeof(ebytePrefix), ebytePrefix, false, TO_PC);
        buildAnswer(0, receivedCount , receivedData, false, TO_PC);
        buildAnswer(0, sizeof(msgPostfix), msgPostfix, true, TO_PC);
        //lastSendTimer[BR_EBYT] = 0;
      } else if(dataDirection == BR_RAK){
        
        foundPartMsg = false;
        echoRak();
      } else{
        buildAnswer(0, receivedCount , receivedData, false, TO_PC);
        buildAnswer(0, sizeof(msgPostfix), msgPostfix, true, TO_PC);
      }
    }
  }
  //clearBuffers();
}  //END_IF_InitCmdFromPC

void initExtraCommands(){
    if (FindCmd(gpson, sizeof(gpson)) == true) {
      setSensState(BR_GPS, true);
      foundExtra = true;
    } else if (FindCmd(gpsoff, sizeof(gpsoff)) == true) {
      setSensState(BR_GPS, false);
      foundExtra = true;
    } else if (FindCmd(ebyton, sizeof(ebyton)) == true) {
      setSensState(BR_EBYT, true);
      foundExtra = true;
    } else if (FindCmd(ebytoff, sizeof(ebytoff)) == true) {
      setSensState(BR_EBYT, false);
      foundExtra = true;
    } else if (FindCmd(rakon, sizeof(rakon)) == true) {
      setSensState(BR_RAK, true);
      foundExtra = true;
    } else if (FindCmd(rakoff, sizeof(rakoff)) == true) {
      setSensState(BR_RAK, false);
      foundExtra = true;
    }else if (FindCmd(ebytePull, sizeof(ebytePull)) == true) {
      fEbytePull(true);
      foundExtra = true;
    } 
}

void echoRak(){
        buildAnswer(0, sizeof(rakPrefix), rakPrefix, false, TO_PC);
        buildAnswer(0, receivedCount , receivedData, false, TO_PC);
        buildAnswer(0, sizeof(msgPostfix), msgPostfix, true, TO_PC);  
        //lastSendTimer[BR_RAK] = 0;  
}
// ---------------------------------------------------- Поиск совпадений с принятыми данными от ПК
boolean FindCmd(byte array1[], byte lengt) {  // true = совпадение данных
  //Serial.print("Check for:");
  //buildAnswer(0, lengt, array1, true, TO_PC);
  //Serial.println("");
  for (byte i = 0; i < lengt; i++) {

    if (receivedData[i] != array1[i]) {  // если один из символов array1 не совпадает с принятыми данными от ПК, то
      delay(5);
      //Serial.print("  Compare :"); Serial.print(receivedData[i]);  Serial.print(" with :"); Serial.print(array1[i]); Serial.print(" - ");
      //Serial.write(receivedData[i]);  Serial.print(":"); Serial.write(array1[i]);
      //Serial.println("");
      return false;
    }
  }
  //Serial.println("FOUND!");
  return true;
}

short findSubStr(byte array1[], byte lengt, byte receivedCount, byte shiftOn) {  // true = совпадение данных
  /*
  Serial.print("Search:"); 
  buildAnswer(0, lengt, array1, true, TO_PC);
  Serial.print(" in:"); 
  buildAnswer(shiftOn, receivedCount, receivedData, true, TO_PC);
  Serial.println("--");
  */

  byte currPosition = shiftOn;
  bool prevEqual = false;
  for (byte i = shiftOn; i < receivedCount; i++) {
    for (byte j = 0; j < lengt; j++) {
      if (receivedData[i] == array1[j]) {  // если один из символов array1 не совпадает с принятыми данными от ПК, то
        prevEqual = true;
      } else {
        prevEqual = false;
      }
    }
    if (prevEqual) {
      return (i);
    }
  }

  return -1;
}
