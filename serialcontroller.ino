char commandSep = ':';

void initSerialController() {
  //Serial.begin(9600);   //Tx0 and Rx0  //Serielle Schnittstelle für PC Starten
  //Serial1.begin(9600);  //Tx1 and Rx1  //Serielle Schnittstelle 1 für Bluetooth starten
  //Serial2;            //Tx2 und Rx2 (Nicht benötigt)
  //Serial3;            //Tx3 und Rx3 (Nicht benötigt)
  //Serial.println("initSerialController done");
}


void updateSerialController() {
  communicate();

}

void sendData(String transmitData) {
  Serial.println(transmitData);
  Serial.flush();
}

void communicate() {
  //first check message handling
  if (Serial.available() <= 0) return;

  String sendText = Serial.readString();
  int sepPos = sendText.indexOf(commandSep);
  if (sepPos < 0) return;

  //split command and parameter
  String command = sendText.substring(0, sepPos);
  String param = sendText.substring(sepPos + 1);

  String res = "0";
  if (command.equals("RESET")) {
    beep();
    resetStorage();
    beep();
  } else if (command.equals("STATUS")) {
    int opt = param.toInt();
    res = getStatus(opt);
  } else if (command.equals("CAL_NORTH")) {
    beep();
    calibrateBike();
    beep();
  } else if (command.equals("UPD_NORTH")) {
    int north = param.toInt();
    if (north >= 0) ee_northAzimuth.set(north);
  } else if (command.equals("UPD_PWR")) {
    int pwr = param.toInt();
    if (pwr >= 0) ee_motorPower.set(pwr);
  } else if (command.equals("UPD_DEGL")) {
    int left = param.toInt();
    if (left < 0) ee_degleft.set(left);
  } else if (command.equals("UPD_DEGR")) {
    int right = param.toInt();
    if (right > 0) ee_degright.set(right);
  } else if (command.equals("UPD_HOMEL")) {
    int left = param.toInt();
    if (left > 0) ee_homeleft.set(left);
  } else if (command.equals("UPD_HOMER")) {
    int right = param.toInt();
    if (right > 0) ee_homeright.set(right);
  } else if (command.equals("UPD_TOL")) {
    int tol = param.toInt();
    if (tol > 0) ee_tolerance.set(tol);
  }else if (command.equals("UPD_TZ")) {
    int tz = param.toInt();
    ee_timezone.set(tz);
  }
  

  //update case
  if (command.indexOf("UPD_") >= 0) {
    initStorage();
    precalculate();
    beep();
  }
  
  sendData(res);

}
