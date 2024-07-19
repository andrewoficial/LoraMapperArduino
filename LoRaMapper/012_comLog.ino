void NumericToByteArr(byte *buferArray, int Arg){
  //Serial.print("do:");  Serial.print(Arg); Serial.println("");
  buferArray[0] = 0;
  buferArray[1] = Arg / 1000;
  buferArray[2] = (Arg - buferArray[1] * 1000) / 100;
  buferArray[3] = (Arg - buferArray[1] * 1000 - buferArray[2] * 100) / 10 ;
  buferArray[4] = Arg - buferArray[1] * 1000 - buferArray[2] * 100 - buferArray[3] * 10 ;
  //Serial.print(buferArray[0]);Serial.print(buferArray[1]);Serial.print(buferArray[2]);Serial.print(buferArray[3]);Serial.println(buferArray[4]);
  buferArray[0] += 48;
  buferArray[1] += 48;
  buferArray[2] += 48;
  buferArray[3] += 48;
  buferArray[4] += 48;
  return *buferArray;
}

void updateLoraMsgRSSI(int pointer, bool hasValue){
  sendMsg[20] = B;
  //sendMsg[21] = A;
  //sendMsg[22] = A;
  //sendMsg[23] = A;
  //sendMsg[24] = A;
  sendMsg[25] = B;
  if(hasValue){
    for(i = 0; i < 4; i++){
      if((receivedData[pointer+i]-48) < 0){
        sendMsg[21+i] = A;
      }else{
        sendMsg[21+i] = receivedData[pointer+i];
      }              
    }
  }
}

void updateLoraMsgTimer(bool hasValue){
  sendMsg[26] = B;
  sendMsg[27] = A;
  sendMsg[28] = A;
  sendMsg[29] = A;
  sendMsg[30] = B;
  if(hasValue){
    for(i = 0; i < 3; i++){
      if((Str_ArrayMyOne[4+i]-48) <= 0){
        sendMsg[27+i] = A;
      }else{
        sendMsg[27+i] = Str_ArrayMyOne[4+i];
      }              
    }
  }
}

void echoReceivedMSG(){
  Serial.println();
  for(i = 30; i < 63; i++){
    Serial.write(receivedData[i]);
  }
  Serial.println();
}






