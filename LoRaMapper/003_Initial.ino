void setupDisplay() {
  screen.begin();
  delay (20);
  screen.setRotation(1); 
  screen.stroke(0, 0, 0);

  screen.availableForWrite();
  screen.flush();
  screen.fillRect(0, 0, 500, 500, ST7735_WHITE); // стираем старое    x y width hight //(355, 117, 90, 23, TFT_BLACK)
  delay (10);
  screen.setTextSize(2);
  screen.println();
  
  screen.print("Device #"); screen.println(deviceNumber);
  screen.println();

  delay (400);
  screen.fillRect(0, 0, 500, 500, ST7735_WHITE);
  screen.text(pStrMyOne, 0, 13);
  screen.text(pStrMyThree, 0, 105);
}

void bootMessage() {
  //Message to com-port
  for (i = 0; i < 32; i++) {
    Serial.print("*");
  }
  Serial.println("*");
  Serial.print("*");
  Serial.print('\t');
  Serial.print("Device: ");
  Serial.print("Lora Mapper");
  Serial.print('\t');
  Serial.println("*");
  Serial.print("*");
  Serial.print('\t');
  buildAnswer(0, sizeof(VersionSW), VersionSW, true, TO_PC);
  Serial.print('\t');
  Serial.println("*");
  for (i = 0; i < 33; i++) {
    Serial.print("*");
  }
  Serial.println("");
}

void setupModul() {
  /*
    ToDo: 
      + check answer
  */



  buildAnswer(0, sizeof(msgPrefix), msgPrefix, false, BR_RAK);
  buildAnswer(0, sizeof(setP2PMode), setP2PMode, false, BR_RAK);
  buildAnswer(0, sizeof(msgPostfix), msgPostfix, true, BR_RAK);
  delay(200);
  checkSomeSerialEvent(BR_RAK);
  delay(400);

  buildAnswer(0, sizeof(msgPrefix), msgPrefix, false, BR_RAK);
  buildAnswer(0, sizeof(setP2pParameters), setP2pParameters, false, BR_RAK);
  buildAnswer(0, sizeof(msgPostfix), msgPostfix, true, BR_RAK);
  delay(200);
  checkSomeSerialEvent(BR_RAK);
  delay(400);

  buildAnswer(0, sizeof(msgPrefix), msgPrefix, false, BR_RAK);
  buildAnswer(0, sizeof(setReciveMode), setReciveMode, false, BR_RAK);
  buildAnswer(0, sizeof(msgPostfix), msgPostfix, true, BR_RAK);
  delay(200);
  checkSomeSerialEvent(BR_RAK);
  delay(400);
  Serial.println("Run...");
}