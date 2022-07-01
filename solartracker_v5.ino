#include <Arduino.h>

#define LOG_BIKEPOS 0
#define LOG_BUTTONS 0
#define LOG_GPS 0
#define LOG_STORAGE 0
#define LOG_SUNPOS 0
#define LOG_TRACKER 0

//tab function definition
//storage
void initStorage();
void resetStorage();

//sunpos
int getAltitude();
int getAzimuth();

int getSolarDegree(int bikedirection, int azimuth, int altitude) ;
//soltracker
void adjustPanel(int paneldeg);
void precalculate();

int getGPSHour();
int getGPSMinute();
float getGPSAzimuth();

#define buzzer 13

#include <EEPROM-Storage.h>
#include <EEPROM.h>

//bikepos
EEPROMStorage<int> ee_northAzimuth(0, 0);
int northAzimuth = 0;

//-1572, 1480, -2230, 952, -1652, 1501
EEPROMStorage<int> ee_calib_1(5, -1572);
EEPROMStorage<int> ee_calib_2(10, 1480);
EEPROMStorage<int> ee_calib_3(15, -2230);
EEPROMStorage<int> ee_calib_4(20, 952);
EEPROMStorage<int> ee_calib_5(25, -1652);
EEPROMStorage<int> ee_calib_6(30, 1501);
int calib_1 = -1572;
int calib_2 = 1480;
int calib_3 = -2230;
int calib_4 = 952;
int calib_5 = -1652;
int calib_6 = 1501;

//soltracker
EEPROMStorage<int> ee_motorPower(35, 150);
int motorPower = 150;
//gps
EEPROMStorage<int> ee_timezone(40, 2);
int timezone = 2;


//manual measured degree of panel
EEPROMStorage<int> ee_degleft(45, -32);
EEPROMStorage<int> ee_degright(50, 39);
int degleft = -32;
int degright = 39;

//values from potentiometer
EEPROMStorage<int> ee_homeleft(55, 26);
EEPROMStorage<int> ee_homeright(60, 245);
EEPROMStorage<int> ee_tolerance(65, 5);
int homeleft = 26;
int homeright = 245;
int tolerance = 5;

//official setup method - run once
void setup () {
  while (!Serial); // for Leonardo/Micro/Zero
  Serial.begin(57600);
  //Wire.begin();
  //resetStorage();
  initAll();
}

//official loop method - run continous
void loop () {
  updateAll();
}


void initAll(){
  initStorage();
  initSerialController();
  initGPS();
  initButtons();
  initSunPos();
  initBikePos();
  initTracker();
  beep();
}


void updateAll() {
  updateSerialController();
  updateGPS();
  updateButtons();
  updateSunPos();
  updateBikePos();
  updateTracker();
}


void beep(){
  tone(buzzer,500,1000);
}

String getStatus(int option){
  switch(option){
    case 1:
      return getPositionStatus();
      break;
    case 2:
      return getConfig();
    default:
      break;
  }
  return "-1";
}

String getPositionStatus(){
    //time - compass - sunpos(az-al)
    char logString[80];
    sprintf(logString,"Time %d:%d Direction: %d Sun - Azimuth %d Altitutde %d",getGPSHour(), getGPSMinute(), getBikeDirection(), getAzimuth(), getAltitude());
    String log = String(logString);
    log.trim();
    return log;
}


String getConfig(){
    //northAzimuth, motorpower,  degleft, degright, homeleft, homeright, tolerance
    char logString[80];
    sprintf(logString,"Motor Power %d, Degree Left %d, Degree Right %d, Sensor Left %d, Sensor Right %d, Tolerance %d", motorPower,  degleft, degright, homeleft, homeright, tolerance );
    String log = String(logString);
    log.trim();
    return log;
}
