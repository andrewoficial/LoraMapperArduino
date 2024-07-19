//*****************************************************************
void EEPROM_Write_Sint1(int p_address, int p_value){
  byte lowByte = ((p_value  >> 0) & 0xFF);
  byte highByte = ((p_value  >> 8) & 0xFF);
  EEPROM.update(p_address, lowByte);
  EEPROM.update(p_address + 1, highByte);  
}
//*****************************************************************
int EEPROM_Read_Sint1(int p_address){
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);  
  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}
//*****************************************************************
unsigned long EEPROM_Read_Ulong1(int p_address) // чтение из EEPROM 4 байта unsigned long
{   
  byte raw[4];
  for(byte i = 0; i < 4; i++) raw[i] = EEPROM.read(p_address+i);
  unsigned long &data = (unsigned long&)raw;
  return data;
}
//*****************************************************************
void EEPROM_Write_Ulong1(int p_address, unsigned long data) // запись в EEPROM 4 байта unsigned long
{
  byte raw[4];
  (unsigned long&)raw = data;
  for(byte i = 0; i < 4; i++) EEPROM.update(p_address+i, raw[i]);
}
//*****************************************************************

void fSaveGPSstate (bool flag){
  if(flag)
    EEPROM_Write_Sint1(5, 1);
  else
    EEPROM_Write_Sint1(5, -1);
}

bool fRestoreGPSstate(){
  Serial.print("GPS state :");
  Serial.println(EEPROM_Read_Sint1(5));

  if(1 == EEPROM_Read_Sint1(5)){
    return true;
  }else{
    return false;
  }
}

void fSaveDevNum(int num){
  EEPROM_Write_Sint1(15, num);
}

int fRestoreDevNum(){
  deviceNumber = EEPROM_Read_Sint1(15);
  return deviceNumber;
}
