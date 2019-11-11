float calculateTurns(float lead) {
  float turns = lead / THREAD;
  return turns;
}

void getScrewPositions() {
  // read current positions
  scZ[0] = 4.01;
  scZ[1] = 7.02;
  scZ[2] = 8.04;
  scZ[3] = 5.03;
  Serial.println("Got screw positions.");
}

void homeMotors() {
  int steps = -10;
  int duration = 100;
  
  zAxisHomed = false;
  while(zAxisHomed == false) {
    moveMotor(motor1, steps, 1);
    // moveMotor(motor2, steps);
    // moveMotor(motor3, steps);
    // moveMotor(motor4, steps);
  }
  if (digitalRead(ZLIMIT) == HIGH) {
    zAxisHomed = true;
    Serial.println("Z homed");
  }
}

void moveMotor(Stepper motor, int steps, int duration) {
    for (int i = 0; i < duration; i++) {
      motor1.step(steps);
    }
  }
