void addSecond(unsigned int interval) {
  unsigned long currentMillis = millis();  //проверяем не прошел ли нужный интервал, если прошел то

  if (currentMillis > humanClock) {
    humanClock = currentMillis + interval;
    devTimer[0] = devTimer[0] + 1;
    devTimer[1] = devTimer[1] + 1;
    devTimer[2] = devTimer[2] + 1;
    workTimer++;
    int delayTimerSend = 10;
    int delayTimerLCD = 1;
    if(! haveGPS){
      delayTimerSend = 1;
    }
    if(subClockLCD > delayTimerLCD){
      subClockLCD = 0;
      lcdShow();
    }else{
      subClockLCD++;
    }
  
    if((subClockSend > delayTimerSend) && gpsIstalled){
      subClockSend = 0;
      if(! isBridget()){
          updateGPS();
      }
    }else{
      subClockSend++;
      lastSendTimer[BR_EBYT]++;
      lastSendTimer[BR_RAK]++;
      lastReceiveTimer[BR_EBYT]++;
      lastReceiveTimer[BR_RAK]++;
    }

    mesVolts();
    
  }
}

void mesVolts(){
  voltageCharge = 0;
  voltageChargeEnd = 0;
  voltage = 0;
  for(short i = 0; i < 20; i++){
    voltage += analogRead(A7);
    voltageCharge += analogRead(A6);
    voltageChargeEnd += analogRead(A5);
  }
  voltage /= 20;
  voltageCharge /= 20;
  voltageChargeEnd /= 20;

  if(voltageCharge < 35)
    voltageCharge = 0;
  
  if(voltageChargeEnd < 35)
    voltageChargeEnd = 0;  

  voltage = voltage * (2.5/1023.0) ;
  voltage *= 2;
  
  voltageCharge /= 2;
  voltageChargeEnd /= 2;
  //Serial.println(voltage);
  //Serial.println(voltageCharge);
  //Serial.println(voltageChargeEnd);

}



void sendLoRaFunc(){
  buildAnswer(0, sizeof(msgPrefix), msgPrefix, false, BR_RAK);
  buildAnswer(0, sizeof(sendMsg), sendMsg, false, BR_RAK);
  buildAnswer(0, sizeof(msgPostfix), msgPostfix, true, BR_RAK);
}