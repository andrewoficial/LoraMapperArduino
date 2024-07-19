// ---------------------------------------------------- Конвертирование одного байта HEX в DEC
byte HexToDec(byte byteH, byte byteL){                  // 0 = ошибка;
      if (byteH >= 48 && byteH <= 57) byteH -= 48;
      else if (byteH >= 65 && byteH <= 70) byteH -= 55;
      else return 0;

      if (byteL >= 48 && byteL <= 57) byteL -= 48;
      else if (byteL >= 65 && byteL <= 70) byteL -= 55;
      else return 0;

      return (byteH << 4) | byteL;
}

// ---------------------------------------------------- преобразование 5 байт ASCII в int
int ASCIIToInt(byte array1[]){
    return ((array1[0]-48)*10000+(array1[1]-48)*1000+(array1[2]-48)*100+(array1[3]-48)*10+(array1[4]-48));
}

void intToArray(int number, int device){
  
}
