#define potiPin 0

#define pinMotor1 6
#define pinMotor2 7
#define pinMotorpower 8

int totaldegree;
int totalsteps;
long stepwidth;
long zeropos;

//index 50 = 0 degree
int calculatedDegrees[101];

//returns stepsvalue for a given degree -50 - 50
int getSteps(int degree) {
  int intdeg = degree;
  if (intdeg < degleft) intdeg = degleft;
  if (intdeg > degright) intdeg = degright;

  int index = intdeg + 50;
  return calculatedDegrees[index];
}

//home left and right
void initTracker() {
  precalculate();
  pinMode(pinMotorpower, OUTPUT);
  pinMode(pinMotor1, OUTPUT);
  pinMode(pinMotor2, OUTPUT);

  if (LOG_TRACKER)  Serial.println("initTracker done");
}

void updateTracker() {
  if(LOG_TRACKER){
    Serial.println("Current Trackerpos "+ String(getCurrentSteps()));
    Serial.println("GPSSpeed "+String(getGPSSpeed()));
  }
  if (automaticButton && isGPSReady() && getGPSSpeed() > 1.0d) {
    int bikedir = getBikeDirection();
    int altitude = getAltitude();
    int azimuth = getAzimuth();
    int paneldeg = getSolarDegree(bikedir, azimuth, altitude);
    if(LOG_TRACKER) Serial.println(" Bike: " + String(bikedir) + " Az: " + String(azimuth) + " Al: " + String(altitude) + " target: " + String(paneldeg));
    adjustPanel(paneldeg);
    return;
  }
  if (panelLeftButton && panelRightButton) {
    adjustPanel(0);
    return;
  }

  if (panelLeftButton) {
    adjustPanel(degleft);
    return;
  }

  if (panelRightButton) {
    adjustPanel(degright);
    return;
  }
  motor_stop();
}

void precalculate() {

  totaldegree = abs(degleft) + abs(degright);
  //homeright is higher than left
  totalsteps = homeright - homeleft;
  stepwidth = totalsteps / totaldegree;
  zeropos = homeright - (degright * stepwidth);
  if (LOG_TRACKER) {
    Serial.print("totaldeg: " + String(totaldegree));
    Serial.print(" degleft: " + String(degleft));

    Serial.print(" degright: " + String(degright));
    Serial.print(" homeleft: " + String(homeleft));
    Serial.print(" homeright: " + String(homeright));
    Serial.print(" totalsteps: " + String(totalsteps));
    Serial.print(" stepwidth: " + String(stepwidth));
    Serial.println(" zeropos: " + String(zeropos));
  }
  for ( int i = -50; i <= 50 ; i++) {
    int index = i + 50;
    int tmpPos = (int) zeropos + i * stepwidth;
    calculatedDegrees[index] = tmpPos;
    if (LOG_TRACKER) Serial.println(String(index) + ": " + String(tmpPos));
  }
}

int getCurrentSteps() {
  return analogRead(potiPin) / 4;
}

void adjustPanel(int paneldeg) {
  int target = getSteps(paneldeg);
  adjustPanelEveryIteration(target);
}

void adjustPanelEveryIteration(int target) {
  int current = getCurrentSteps();
  //check for tolerance
  int distance = abs(target - current);
  //check for tolerance
  if ( distance < tolerance) {
    motor_stop();
    return;
  }

  float percDist = (float) distance / totalsteps;

  if (LOG_TRACKER) {
    Serial.println("currentstep: " + String(current) + " targetstep: " + String(target) + " tolerance: " + String(tolerance));
    Serial.println("distance: " + String(distance) + " percDist: " + String(percDist, 5) + " totalsteps: " + String(totalsteps));
  }
  //adjust speed as we get closer to target
  double power = motorPower;
  //if (percDist < 0.05) power = motorPower * 0.9;
  //if(percDist < 0.5) power = motorPower * 0.7;
  //if(percDist < 0.1) power = motorPower * 0.8;


  if (LOG_TRACKER)  Serial.println("power: " + String(power) + " motorpower: " + String(motorPower));

  if ( target > current ) {
    driveMotor(1, power);
  } else if (target < current) {
    driveMotor(-1, power);
  } else {
    driveMotor(0, 0);
  }
}

void driveMotor(int dir, int power) {
  switch (dir) {
    case 1:
      motor_cw(power);
      if (LOG_TRACKER)  Serial.println("Motor - CW");
      break;
    case -1:
      motor_ccw(power);
      if (LOG_TRACKER)  Serial.println("Motor - CCW");
      break;
    default:
      motor_stop();
      if (LOG_TRACKER)  Serial.println("Motor - STOP");
      break;
  }
}

void motor_cw(int power) {
  digitalWrite(pinMotor1, HIGH);
  digitalWrite(pinMotor2, LOW);
  analogWrite(pinMotorpower, power);
}

void motor_ccw(int power) {
  digitalWrite(pinMotor1, LOW );
  digitalWrite(pinMotor2, HIGH );
  analogWrite(pinMotorpower, power);
}

void motor_stop() {
  digitalWrite(pinMotor1, LOW );
  digitalWrite(pinMotor2, LOW );
  analogWrite(pinMotorpower, 0);
}

//for optimal positioning we must be in an angle of 90deg to the sun
int getSolarDeg(int altitude ) {
  return 90 - altitude;
}

//we get an output of [maxleft,0, maxright]
int getSolarDegree(int bikedirection, int azimuth, int altitude) {
  int valDeg = getSolarDeg(altitude);

  int nulledSundegree = azimuth - bikedirection;
  //correct into range 0-359
  if (nulledSundegree < 0) nulledSundegree += 360;
  if (nulledSundegree >= 360) nulledSundegree -= 360;
  float perc = 0;

  //lets use a sinus function to determine on which side the sun is
  float sinusvalue = sin(nulledSundegree * DEG_TO_RAD);
  //angle of panel should only be maximum when 90 or 270 degree to the sun
  int targetdeg = (int) (sinusvalue * valDeg + 0.5);

  //limit in case degree is too big
  if (targetdeg > degright) {
    targetdeg = degright;
  }
  if (targetdeg < degleft) {
    targetdeg = degleft;
  }
  return targetdeg;
}
