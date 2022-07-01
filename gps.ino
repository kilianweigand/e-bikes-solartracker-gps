#include <TinyGPSPlus.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define RXPin 3
#define TXPin 4
#define GPSBaud 9600

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
int validGPSData = false;

float getGPSAzimuth(){
  return gps.course.deg();
}
double getGPSSpeed(){
  return gps.speed.kmph();
}
int getGPSHour(){
  int hour = gps.time.hour()+timezone;
  if(hour>=24) hour-= 24;
  return hour; 
}
int getGPSMinute(){
  return gps.time.minute(); 
}

void initGPS(){
  ss.begin(GPSBaud);
  if (LOG_GPS) Serial.println("initButtons done");
}

bool isGPSReady(){
  if (LOG_GPS){
    Serial.print("isGPSReady - location:"+String(gps.location.isValid()));
    Serial.print(" time:"+String(gps.time.isValid()));
    Serial.println(" course:"+String(gps.course.isValid()));
  }
  return gps.location.isValid() && gps.time.isValid() && gps.course.isValid();
}

void updateGPS(){
  while (ss.available() > 0) {
    char cc = ss.read();
    if (LOG_GPS) Serial.write(cc);
    gps.encode(cc);
  }
  //beep if it changes
  bool check = isGPSReady();
  if(check!=validGPSData){
    beep();
    validGPSData = check;
  }
}
