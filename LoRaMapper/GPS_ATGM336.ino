void updateGPS(){

  gps.read(); 
  
  if(!gps.errPos){ 
    longitude = gps.longitude;
    latitude = gps.latitude;
    
    accuracyX = (int) gps.HDOP;
    accuracyZ = (int) (gps.PDOP * 10);
    accuracyY = (int) gps.VDOP;
    activeSatellites = gps.satellites[1];
    observedSatellites = gps.satellites[0];
    currentMessageNumber = 20;
    totalMessages = 20;
    float voltage= analogRead(A3) * (5.0 / 1025.0);
    batteryLevel = (int) (voltage * 100);
    sendMessages();
    haveGPS = true;
  }else{
    Serial.println("Err gps");
    haveGPS = false;
  }
}

void sendMessages(){
    totalMessages = 1;
    for(int i = 0; i < totalMessages; i++){
        currentMessageNumber = i + 1;
        createMessage();  
          Serial.println("snd EBYTE");
          fEbytePull(false);
          delay(250);
          checkSomeSerialEvent(BR_EBYT);
          fEbytePull(false);
          delay(250);
          
          buildAnswer(0, 34 , message, false, BR_EBYT);
          buildAnswer(0, sizeof(msgPostfix), msgPostfix, true, BR_EBYT);
          lastSendTimer[BR_EBYT] = 0;

          delay(700);

          Serial.println("snd RAK");
          //Serial.println("  Setup Transmite");
          fSetTransmiteMode();
          delay(50);

          //Serial.println("  Run Mesage");
          buildAnswer(0, sizeof(msgPrefix), msgPrefix, false, BR_RAK);
          buildAnswer(0, sizeof(sendP2PWord), sendP2PWord, false, BR_RAK);
          buildAnswer(0, 34 , message, false, BR_RAK);
          buildAnswer(0, sizeof(msgPostfix), msgPostfix, true, BR_RAK);
          checkSomeSerialEvent(BR_RAK);
          lastSendTimer[BR_RAK] = 0;
          clearBuffers();
          delay(100);

          //Serial.println("  Setup Receive");
          fSetReceiveMode();
          delay(50);
    }
}
void createMessage() {
  for(int i = 0; i < 47; i++){
    message[i] = 0;
  }
  //memset(message, 0, sizeof(message));
  
  // Заполнение массива данными
  //Serial.print("longitude: "); 
  //Serial.println(longitude); 
  longitude = gps.longitude;
  formatFloatToHex(message, longitude, 0);
  //printMessagePart(message, 0, 20);

  //Serial.print("latitude: "); 
  latitude = gps.latitude;
  formatFloatToHex(message, latitude, 8);
  //printMessagePart(message, 0, 20);

  //formatByteToHex(message, accuracyX, 16);
  //Serial.print("accuracyX: "); 
  //Serial.println(accuracyX); 
  //printMessagePart(message, 16, 18);

  //formatByteToHex(message, accuracyZ, 18);
  //Serial.print("accuracyZ: "); 
  //Serial.println(accuracyZ); 
  //printMessagePart(message, 18, 20);

  //formatByteToHex(message, accuracyY, 20);
  //Serial.print("accuracyY: "); 
  //Serial.println(accuracyY); 
  //printMessagePart(message, 20, 22);

  formatByteToHex(message, activeSatellites, 16);
  //Serial.print("actSat: "); 
  //Serial.println(activeSatellites); 
  //printMessagePart(message, 22, 24);

  //formatByteToHex(message, observedSatellites, 24);
  //Serial.print("obSat: "); 
  //Serial.println(observedSatellites); 
  //printMessagePart(message, 24, 26);

  formatByteToHex(message, currentMessageNumber, 18);
  //Serial.print("mNum: "); 
  //Serial.println(currentMessageNumber); 
  //printMessagePart(message, 26, 28);

  formatByteToHex(message, totalMessages, 20);
  //Serial.print("tNum: "); 
  //Serial.println(totalMessages); 
  //printMessagePart(message, 28, 30);

  formatUIntToHex(message, deviceNumber, 22);
  //Serial.print("serNum: "); 
  //Serial.println(serialNumber); 
  //printMessagePart(message, 30, 38);

  formatUIntToHex(message, batteryLevel, 26);//30
  //Serial.print("batLev: "); 
  //Serial.println(batteryLevel); 
  //printMessagePart(message, 38, 42);

    // Вычисление CRC16

    //printMessagePart(message, 0, 40);
    //printMessagePart(message, 0, 30);

    uint16_t crc = calculateCRC16(message, 30);
    //Serial.print("CRC: ");
    //Serial.println(crc, HEX);

    formatUIntToHex(message, crc, 30);
    //printMessagePart(message, 0, 34);
    
    

}

// Функции для форматирования данных в HEX и заполнения массива
void formatFloatToHex(char* buffer, float value, int startIndex) {
  long intValue = (long)(value * 100000);
  //Serial.print("Multiplied: ");
  //Serial.println(intValue);
  formatLongToHex(buffer, intValue, startIndex);
}

// Функция для вычисления CRC16
uint16_t calculateCRC16(const char* data, int length) {
    uint16_t crc = 0xFFFF; // Начальное значение CRC

    for (int i = 0; i < length; i++) {
        uint8_t byte = data[i];
        uint8_t index = (crc >> 8) ^ byte;
        crc = (crc << 8) ^ CRC16_TABLE[index];
    }

    return crc;
}

void formatByteToHex(char* buffer, byte value, int startIndex) {
  sprintf(buffer + startIndex, "%02X", value);
}

void formatULongToHex(char* buffer, unsigned long value, int startIndex) {
  sprintf(buffer + startIndex, "%06lX", value);
}

void formatUIntToHex(char* buffer, int value, int startIndex) {
  sprintf(buffer + startIndex, "%04X", value);
}

void formatLongToHex(char* buffer, long value, int startIndex) {
  if (value < 0) {
    value = -value;
    value |= 0x80000000;
  }
  //Serial.print("Minused: ");
  //Serial.println(value);
  sprintf(buffer + startIndex, "%08lX", value);
}

// Функция для печати части сообщения
void printMessagePart(char* buffer, int start, int end) {
  Serial.print("msgPrt[");                                          
  for(int i = start; i < end; i++){
    Serial.write(buffer[i]);
  }
  Serial.println("]");  
}