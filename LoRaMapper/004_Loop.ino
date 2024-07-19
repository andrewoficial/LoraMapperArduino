void loop() {

    if (sensState == BR_GPS) {
      checkSomeSerialEvent(BR_GPS);
      delay(100);
    }
  checkSomeSerialEvent(BR_EBYT);
  checkSomeSerialEvent(TO_PC);  
  checkSomeSerialEvent(BR_EBYT);
  checkSomeSerialEvent(BR_RAK);
  checkSomeSerialEvent(BR_EBYT);
  addSecond(900);
  //Serial.println(humanClock);
}
