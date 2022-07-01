
void clearStorage() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0xFF);
  }
}

void initStorage() {
  northAzimuth = ee_northAzimuth.get();
  calib_1 = ee_calib_1.get();
  calib_2 = ee_calib_2.get();
  calib_3 = ee_calib_3.get();
  calib_4 = ee_calib_4.get();
  calib_5 = ee_calib_5.get();
  calib_6 = ee_calib_6.get();

  motorPower = ee_motorPower.get();
  timezone = ee_timezone.get();
  
  degleft = ee_degleft.get();
  degright = ee_degright.get();

  homeleft = ee_homeleft.get();
  homeright = ee_homeright.get();
  tolerance = ee_tolerance.get();
  if (LOG_STORAGE) Serial.println("initStorage done");
}

void resetStorage() {
  writeDefaultsToStorage();
  initStorage();
}

void writeDefaultsToStorage() {

  //bikepos
  ee_northAzimuth.set(0);
  ee_calib_1.set(-1572);
  ee_calib_2.set(1480);
  ee_calib_3.set(-2230);
  ee_calib_4.set(952);
  ee_calib_5.set(-1652);
  ee_calib_6.set(1501);
  //soltracker
  ee_motorPower.set(150);

  ee_timezone.set(2);

  //manual measured degree of panel
  ee_degleft.set(-32);
  ee_degright.set(39);

  //values from potentiometer
  ee_homeleft.set(26); //-32 deg
  ee_homeright.set(245); //40
  ee_tolerance.set(5);
  if (LOG_STORAGE) Serial.println("writeDefaultsToStorage done");
}
