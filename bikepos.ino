int bikedirection = 0;

void updateBikePos() {
  //apply correction
  bikedirection = northAzimuth +  getGPSAzimuth();
  if (bikedirection > 359) bikedirection -= 360;
  if (LOG_BIKEPOS) Serial.println("updateBikePos bikedirection: " + bikedirection);
}

int getBikeDirection() {
  return bikedirection;
}

void calibrateBike() {
  int azimuth = getGPSAzimuth();
  //save to memory
  ee_northAzimuth.set(azimuth);
  if (LOG_BIKEPOS) Serial.println("calibrateBike azimuth correction: " + azimuth);
  initStorage();
  beep();
}

void initBikePos() {
  if (LOG_BIKEPOS) Serial.println("initBikePos done");
}
