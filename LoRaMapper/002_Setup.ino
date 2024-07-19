void setup() {
  wdt_disable();
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  deviceNumber = fRestoreDevNum();
  //fSaveDevNum(66);
  gpsIstalled = fRestoreGPSstate();
  //Serial.setTimeout(300);
  //Serial1.setTimeout(300);
  //Serial2.setTimeout(200);
  //Serial3.setTimeout(500);



      pinMode(22, OUTPUT);
    digitalWrite(22, LOW);

      pinMode(24, OUTPUT);
    digitalWrite(24, LOW);    

  if(gpsIstalled){
    SettingsGPS.begin(Serial3);
    gps.begin(Serial3);  
    SettingsGPS.baudrate(9600);  
    SettingsGPS.system(GPS_GP, GPS_GL, GPS_BD); 
    SettingsGPS.composition(NMEA_RMC, NMEA_GGA);
    SettingsGPS.updaterate(10);   
  }else{
    haveGPS = false;
  }
  
  //analogReference(EXTERNAL);

  setupDisplay();
  bootMessage();
  setupModul();
}
