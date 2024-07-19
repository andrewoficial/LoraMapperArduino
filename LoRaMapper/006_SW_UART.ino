void checkSomeSerialEvent(int num) {
  if (serialPorts[num]->available()) {
    if (anySerialRead(num)) {  // принимаем данные
      dataDirection = num;
      initCommand();
      if (isBridget()) {
        if (num == TO_PC) {
          dataDirection = sensState;  //Если пришло от ПК, то отправить в другой порт
          fEchoReceived();
        } else if (num == sensState) {  //Если пришло с порта, с которым мост
          dataDirection = TO_PC;
          fEchoReceived();
        }
      }
    }
  }
}

boolean anySerialRead(int num) {  // true = прием закончен
  receivedCount = 0;
  while (serialPorts[num]->available()) {
    while(serialPorts[num]->available()){
      receivedData[receivedCount] = serialPorts[num]->read();  // читаем по одному байту
      delay(10);                                               // что бы успел завершиться прием
      receivedCount++;                                         // увеличиваем счетчик принятых байт с COM port
      if (receivedCount == countBytesPcmax)                    // если счетчик байт = максимальному байт в буфере receivedCount
        return true; 
    }
    timeOutAnySerial(10, num);
  }
  return true;
}

//----------------------------------------------------- TimeOut
void timeOutAnySerial(unsigned long timeout, int num) {
  for (int tmp = 0; tmp > 200; tmp++) {  // ждем пока не появятся данные
    delay(timeout);
    if (serialPorts[num]->available() > 0) {
      delay(10);
      break;
    }
  }
}


void buildAnswer(short from, short to, byte *messagePart, bool andSend, short direction) {
  for (int i = from; i < to; i++) {
    toSendData[toSendCount] = messagePart[i];
    toSendCount++;
  }
  if (andSend) {
    sendSomeUart(direction);
  }
}

void sendSomeUart(short direction) {

  for (short i = 0; i < toSendCount ; i++) {
      serialPorts[direction]->write(toSendData[i]);
  }
  toSendCount = 0;//сбрасываем указатель после отправки
  delay(10);
}

void clearBuffers(){
  for(int i = 0; i < countBytesPcmax; i++){
    receivedData[i] = 0;
    toSendData[i] = 0;
  }
}